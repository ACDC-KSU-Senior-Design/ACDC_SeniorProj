//#include "hidef.h"
//#include "derivative.h"      /* derivative-specific definitions */
#include "main.h"
#include "3110Driver.h"
#include "BoardTest.h"
//#include "Uart.h"    

   

// * Global Variables and arrays *
uint8 DebugArray[15];                        // Global array for 3110 status registers
unsigned char RXBuffer[64][16];              // [# of buffers][16 bytes]
char DebugArrayStrings[][10] ={              // Status register dump message headers
   "CTR0 =\t",
   "CTR1 =\t",
   "BTR0 =\t",
   "BTR1 =\t",
   "TEC =\t",
   "REC =\t",
   "MESSTAT=",
   "STATF =\t",
   "ERR =\t",
   "INTF =\t",
   "INTE =\t",
   "STATFE=\t",
   "GPINE =\t",
   "TIMERUB=",
   "TIMERLB="
};

#define ID1 1;
#define ID2 2;
#define ID3 3;
#define ID4 4;
#define ID5 5;
#define ID6 6;
#define ID7 7;
   
unsigned char TXBuffer [16];                 // Transmit Buffer

unsigned char MODES,OPTION;
uint8 g_RXBuffer[16];            // Global receive array loaded by interrupt
uint8 g_RecRdy;                  // Flag to indicate new CAN data avail from interrupt
uint8 g_TXCPLTIE;                // Global flag for TXCPLTIE status from interrupt


//------------------------ MAIN ------------------------------------------------
void main() {

PeriphInit();                   // initialize PLL, SPI Clocks and GPIO
GPIO_InitInterrupts();          // initialize PPO/PP1 interrupts

Timer_Init();                  // initialize T0 for the main 100us periodic system interrupt
                               //clear all possible false interrupts just in case
PIFP_PIFP0 = 1;                //clear PPO/3110 Interrupt 
PIFP_PIFP1 = 1;                //clear PP1/3110 Stat Interrupt 
PIFP_PIFP2 = 1;                //clear PP2/3110 GP2 
PIFP_PIFP3 = 1;                //clear PP3/3110 GP1  

int ID = ID1;

EnableInterrupts; 

UartConfig();                    // configure the UART, 115200B, 8 bits, no parity

crlf();

HW_RESET();                      // issue a hardware reset to the HI-3110
 
for(;;) {

   switch (MODE) {         
                                
        case TRANSMIT_MODE:      // 1  Transmit an incrementing 32bit counter value with Standard Frame Format
                Transmit_Mode(STANDARDFRAME);
                break;
                       
        case RECEIVE_MODE:       // 2  Receive Mode with comparing 32bit counter, demo SLEEP Mode.                
                Receive_Mode();
                break;

                                                      
        default:break;        
        };
      
    }; // for

}	// main end


void Transmit_Mode(unsigned char frameType){
   unsigned char STAT_F;
   static unsigned char MessageCount, i, TxPaused;
   uint8 TransmitHistoryFIFO[8][3];                   // History FIFO 8 deep 3 bytes each

      
   union Buffer32 {
      
      long Word32;
      unsigned char PayLoad[8];
      };

   union Buffer32 BigCounter;

   char T8Header[]={
       
       0x12,           // FIFO Write command
       0x04,           // Message Tag:                   [MT7,MT6,MT5,MT4 ,MT3,MT2,X,X]
       0x47,           // Base Identifier                [ID28,ID27,ID26,ID25, ID24,ID23,ID22,ID21]
       0x20,           // Base Identifier+RTR+IDE        [ID20,ID19,ID18, RTR,IDE,X,X,X]
       0x08            // Reservered bit+DLC             [r0,X,X,X, DLC3, DLC2,DLC1,DLC0]
   };
   

   
    char T8HeaderExt[]={
       
       0x12,           // FIFO Write command
       0x04,           // Message Tag:                   [MT7,MT6,MT5,MT4 ,MT3,MT2,X,X]
       0x47,           // Base Identifier                [ID28,ID27,ID26,ID25, ID24,ID23,ID22,ID21]
       0x28,           // Base Identifier+RTR+IDE        [ID20,ID19,ID18, RTR,IDE,X,X,X]
       0x37,           // Base Identifier                [ID14,ID13,ID12,ID11, ID10,ID09,ID08,ID07]       
       0x26,           // Base Identifier                [ID06,ID05,ID04,ID03, ID02,ID01,ID00,RTR]       
       0x08            // Reservered bit+DLC             [r0,X,X,X, DLC3,DLC2,DLC1,DLC0]
   };
 
   TxPaused=0;                // start out not paused
   BigCounter.Word32 = 0;     // clear all 8 bytes of the data field.  
   LED_CTL(LED_1,ON);
   Delay100us(K_1SEC);        // blink for 100ms                                        
   LED_CTL(LED_1,OFF);

   UartConfig();
   
   if(RDY != Init3110(NORMAL, TTDIV0, WAKEUP_OFF, RESET_NO, BOR_OFF))
      {
          for(;;);                        // deadloop on fail
      }      

 //  Allow TXCPLT to generate interrupt on GP1
   GPINE(GPIN2_RXFIFO, GPIN1_TXCPLT); 
   (void) R_Register (INTF_R);            // clear all INTF flags  
   PIFP_PIFP3 = 1;                         //clear GP1 Interrupt
   g_TXCPLTIE=FALSE; 

   (void)memset(TXBuffer, 0, sizeof(TXBuffer));                   // clear the buffer
   
   if(frameType==STANDARDFRAME)
      {
         (void)memcpy(TXBuffer, T8Header, sizeof(T8Header));      // load the header       
      }
   else if(frameType==EXTENDEDFRAME)
      {
         (void)memcpy(TXBuffer, T8HeaderExt, sizeof(T8HeaderExt)); // load the header       
      }
   


   PackPayLoad8(frameType, BigCounter.PayLoad);                    // pack in the first payload into TXBuffer[]
   
   BigCounter.Word32 = 0;                                          // initilize the 32bit counter
   BigCounter.PayLoad[4]=4;
   BigCounter.PayLoad[5]=5;
   BigCounter.PayLoad[6]=6;
   BigCounter.PayLoad[7]=7;
   
                                  
   W_STATFE(TXMTYIE);                // Configure STAT for Transmit FIFO empty status [1E 40]
                                     // This will generate an interrupt from the STAT pin but do nothing.

   TXEN = LO;  

    
   for(;;){
   MessageCount=0;
                      
   for(;;) {                         // TEST Transmit loop 
                    
       do {
            /*
            if(!SW3)                         // print out the History FIFOs                                           
               {
               for(i=0; i<8; i++){           
                  MultiByteRead(0xEE,3, TransmitHistoryFIFO[i]); // Fetch the History FIFO with our local buffer
                  }
               for(i=0; i<8; i++){
                   printHistoryData(TransmitHistoryFIFO[i]); 
                   }
               crlf();                       
               while(SW4);                   // start again if Button-4 pressed
               }*/               
               
                      
             STAT_F = STATF();             // Get Status Register Contents  [E2,00]                 
           } while (STAT_F & TXFULL);      // Wait until FIFO not full before loading next byte
 
   // FIFO is not full so ok to load more frames
   for(i=0; i<8; i++)                      // Load up to 1-8 Frames before sending
      {               
         if(frameType==STANDARDFRAME)
            {
               TransmitStandardFrame(TXBuffer);    // Transmit the 0x12 command + 12 bytes of frame data
               PORTT= ~TXBuffer[7];    
            }
         else if(frameType==EXTENDEDFRAME)
            {
               TransmitExtendedFrame(TXBuffer);    // Transmit the 0x12 command + 14 bytes of frame data
               PORTT= ~TXBuffer[9];         
            }

         TXEN = HI;
         PORTA |= 0x80;                                     // pulse high for scope
         PORTA &= 0x7F;           
         BigCounter.Word32++;                               // increment the HUGE counter               
         PackPayLoad8(frameType, BigCounter.PayLoad);       // pack in the payload into TXBuffer[]          
      }
      
  // Wait here for interrupt to set transmission completion just for the first frame out.
  // This is not really required here we just want to demonstrate this TXCPLTIE interrupt completion bit

  while(g_TXCPLTIE==FALSE);         // OPtional - wait for at least one trasmission complete...
  g_TXCPLTIE=FALSE;
    
   PORTB |= 0x08;                      // pulse PB3 high for scope
   while(0 == (STATF() & 0x80));       // wait for Transmit FIFO Empty - all frames sent out.     
   PORTB &= 0xF7;      
  
   if(PORTA & OPT1) {                  // Optional - Just some delay for scope viewing purposes     
         Delay100us(4);
      }    

  } // for message     
 } // loop    
}  // Transmit Mode End

void Receive_Mode(void){
   unsigned char STAT_F;
   static unsigned char ByteCount,i;

   static uint8 MessageCount;

   unsigned char RXBuffer16[16];                // Temp buffer to hold frame data
   	
   union Buffer32 {

   long Word32;
      unsigned char PayLoad[8];
      };

   union Buffer32 BigCounter; 
   	 
   BigCounter.Word32 = 0;
   
   UartConfig();
   

// Initialize the HI-3110 including the bitrate registers
   if(RDY != Init3110(NORMAL, TTDIV0, WAKEUP_OFF, RESET_NO, BOR_OFF))
      {
          for(;;); // lock if failed
      }   


   GPINE(GPIN_RXFFULL, GPIN_RXFMPTY);                                       
   W_STATFE(RXFMTYIE);              // Configure STAT for Receive FIFO empty status [1E 02] 
   (void) R_Register (INTF_R);      // clear all INTF flags 
      

   PORTA |= 0x80;
  
   for(MessageCount=0; MessageCount< 64; MessageCount++) {   
      for (ByteCount=0; ByteCount<16; ByteCount++) 
         {
          RXBuffer[MessageCount][ByteCount] = 0;            // Clear the whole Frame Buffer 
         }   
   }
   
   MessageCount =0;

          
   for(;;) 
      {                                      
        
       do {
            PORTA &= 0x7F;              
                  
       
               DisableInterrupts;    
               STAT_F = STATF();             // Get Status Register Contents  [E2,00]   
               EnableInterrupts;           
              
          } while (STAT_F & RXFMTY);      // Wait until RX FIFO not full before fetching next message

 
      DisableInterrupts;
      PORTA |= 0x80; 
      FetchFIFOFrame(RXBuffer16);       // pass the buffer location to get filled with data
      (void)memcpy(RXBuffer[MessageCount],RXBuffer16,sizeof(RXBuffer16));  // copy frame to large array for safe keeping

      PORTT= ~RXBuffer[MessageCount][10];  // write out to LEDs1-4  we don't care about the other nibble              

      EnableInterrupts;  
                                            

      BigCounter.Word32++;                        // increment the HUGE counter      
 
      MessageCount++;
      if(MessageCount==64)
         MessageCount=0;

      
         
   } // loop forever... 
     
}  // Receive Mode End