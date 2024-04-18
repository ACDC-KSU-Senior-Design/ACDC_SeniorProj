/*       3110Driver.c
 *       Holt HI-3110 Demonstration Program 
 *
 *
 *      	This file demonstrates a simple ANSI C program structure for 
 *			the initializing and using a Holt HI-3110 device.
 *
 *
 *
 *			HOLT DISCLAIMER
 *
 *			THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *			EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO WARRANTIES
 *			OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *			NONINFRINGEMENT. 
 *			IN NO EVENT SHALL HOLT, INC BE LIABLE FOR ANY CLAIM, DAMAGES
 *			OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
 *			OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 *			SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *			Copyright (C) 2009 - 2010 by HOLT, Inc.
 *			All Rights Reserved.
 */
 
#include "derivative.h"      /* derivative-specific definitions */ 
#include "3110Driver.h"
#include "main.h"
#include "common.h"

#define EXTERNAL_TRANSCEIVER 0    // HI-3110 = 0, HI-3111 w/ External NXP TJA1040 =1

unsigned char STAT_F;
unsigned char RATE;
const unsigned char BitRate[] = {0x40, 0x41, 0x43, 0x47, 0x0B};    // Bit Rates: 1mbps, 500k, 250k, 125, 83.3k

/*
Array Filter and Mask data for Filter 0
Filter ID byte format: 
[cmd], [ID28-ID21], [ID20-ID18,RTR,IDE,ID17-ID15],[ID14-ID7], [ID6-ID0,X], [DataByte 1], [DataByte 2]
This demonstration code will transmit ID28-ID21 = 0x47. So if you change the filter from this value
or transmit a different value the message will be filtered.
*/

// CAN Bus acceptance filters
unsigned const char Filters[8][7] ={                   // Filter data
   {W_FILTER0, 0x47,0x00, 0x00, 0x00, 0x00,0x00},      // Filter 0
   {W_FILTER1, 0x47,0x00, 0x00, 0x00, 0x00,0x00},
   {W_FILTER2, 0x47,0x00, 0x00, 0x00, 0x00,0x00},
   {W_FILTER3, 0x47,0x00, 0x00, 0x00, 0x00,0x00},
   {W_FILTER4, 0x47,0x00, 0x00, 0x00, 0x00,0x00},
   {W_FILTER5, 0x47,0x00, 0x00, 0x00, 0x00,0x00}, 
   {W_FILTER6, 0x47,0x00, 0x00, 0x00, 0x00,0x00}, 
   {W_FILTER7, 0x47,0x00, 0x00, 0x00, 0x00,0x00},       // Filter 7      
   };
   
// CAN Bus acceptance masks
unsigned const char Masks[8][7] ={                       // Mask data
   {W_MASK0, 0xFF,0x00,	0x00, 0x00, 0x00,0x00},          // Mask 0
   {W_MASK1, 0xFF,0x00,	0x00, 0x00, 0x00,0x00},
   {W_MASK2, 0xFF,0x00,	0x00, 0x00, 0x00,0x00},
   {W_MASK3, 0x00,0x00,	0x00, 0x00, 0x00,0x00},          // Accept everything with filter 3
   {W_MASK4, 0xFF,0x00,	0x00, 0x00, 0x00,0x00},
   {W_MASK5, 0xFF,0x00,	0x00, 0x00, 0x00,0x00}, 
   {W_MASK6, 0xFF,0x00,	0x00, 0x00, 0x00,0x00}, 
   {W_MASK7, 0xFF,0x00,	0x00, 0x00, 0x00,0x00},          // Mask 7      
   };


// ------------------------------------------------------------------
//  Initialize the HI-3110
//  Usage:
//    uint8 char Init3110(uint8 MODE, uint8 TTDIV, uint8 WAKEUP, uint8 RESET, uint8 BOR);
//    Return value = OK
//    
//    MODE: NORMAL, MONITOR, SLEEP, INIT
//    TTDIV: TTDIV0, TTDIV2, TTDIV4, TTDIV8
//    WAKEUP: WAKEUP_ON, WAKEUP_OFF
//    RESET: RESET, RESET_NO
//    BOR: BOR_ON, BOR_OFF
// 
// Example: 
//    if(!Init3110(NORMAL, TTDIV0, WAKEUP_OFF, RESET, BOR_OFF))
//       {
//          LED8 = ON;     turn on fail LED
//          While (1);     deadloop on fail
//       }
//    
// 
// ------------------------------------------------------------------
uint8 Init3110(const mode, uint8 ttdiv, uint8 wakeup, uint8 reset, uint8 bor){
   unsigned char cmd;
   
   MASTER_RESET_SPI();        // perform a SW master reset

   WriteFiltersMasks();       // Write all the filters and masks registers for receiver modes

   RATE_SELECT();             // set Bitrate according to dip switches
   cmd = mode<<5;
   cmd |= ttdiv;
   cmd |= (wakeup<<4);
   cmd |= (reset <<3);
   cmd |= (bor <<2);
   W_CTRL0 (cmd);             // initialize the new mode and settings in CTRL0
   
  if(PORTA & OPT2) {          // Enable the Filter0/Mask0 if option OPT2 open(high)
                              // CAUTION: OptionSW_1 is actually marked OPT2 on the board!     
   W_CTRL1 (0x10);				// Enable the Filters/Masks
   }
   return  STATF();
}

// ------------------------------------------------------------------
// Return the status of Sleep mode
// ------------------------------------------------------------------
uint8 getSleepStatus(void)
{
   static uint8 mode;               // static allows to see local var in the debugger
   mode = 0xE0 & R_CTRL0();         // read and save the CTRL0 Register mode bits  

   if(mode==SLEEP_STATUS)
      {
         return TRUE;
      } else 
      return FALSE;    
   
}

// ------------------------------------------------------------------
// Return the status of Monitor mode
// ------------------------------------------------------------------
uint8 getMonitorStatus(void)
{
   static uint8 mode;               // static allows to see local var in the debugger
   mode = 0xE0 & R_CTRL0();         // read and save the CTRL0 Register mode bits 

   if(mode==MONITOR_STATUS)
      {
         return TRUE;
      } else 
      return FALSE;    

}

// ------------------------------------------------------------------
// Return the status of Normal mode
// ------------------------------------------------------------------
uint8 getNormalStatus(void)
{
   static uint8 mode;               // static allows to see local var in the debugger
   mode = 0xE0 & R_CTRL0();         // read and save the CTRL0 Register mode bits 

   if(mode==NORMAL_STATUS)
      {
         return TRUE;
      } else 
      return FALSE;    
   
}

// ------------------------------------------------------------------
// Write SPI Command with a Value to HI-3110
// ------------------------------------------------------------------
void W_CommandValue (uint8 cmd, uint8 value){
  uint8 dummy; 
  	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK; // disable auto /SS output, reset SPI0 Mode Fault 
  SPI0_nSS = 0;                             // assert the SPI0 /SS strobe
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = cmd;                             // SPI  command 
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = value;                           // Reset values     
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF

  SPI0_nSS = 1;                             // negate the SPI0 /SS strobe	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;    // enable auto /SS output, set /SS Output Enable 
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
}

// ------------------------------------------------------------------ 
// Write to GPINE Register to configure the GP1 and GP2 Pin outputs of the 3110.
// ------------------------------------------------------------------
void W_GPINE (unsigned char stat) {
  unsigned char dummy; 
  	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK; // disable auto /SS output, reset SPI0 Mode Fault 
  SPI0_nSS = 0;                             // assert the SPI0 /SS strobe
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = 0x22;                            // SPI GPINE command 
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = stat;                            // Reset values     
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF

  SPI0_nSS = 1;                             // negate the SPI0 /SS strobe	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;    // enable auto /SS output, set /SS Output Enable 
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
} 

// ------------------------------------------------------------------
// Write to Status Flag Enable Register to configure the STAT Pin output.
// ------------------------------------------------------------------
void W_STATFE (unsigned char stat) {
  unsigned char dummy; 
  	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK; // disable auto /SS output, reset SPI0 Mode Fault 
  SPI0_nSS = 0;                             // assert the SPI0 /SS strobe
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = 0x1E;                            // SPI command 
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF
  dummy = SPI0SR;                           // clear SPI status register    
  SPI0DR = stat;                            // Reset values     
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;                           // read Rx data in Data Reg to reset SPIF

  SPI0_nSS = 1;                             // negate the SPI0 /SS strobe	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;    // enable auto /SS output, set /SS Output Enable 
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
} 



/* ------------------------------------------------------------------
/  Read HI-3110 Control 0  Register Read Function
/  ------------------------------------------------------------------
Argument(s):  none

     Return:  8-bit Control 0  Register Values 
*/
unsigned char R_CTRL0 () {                   
  unsigned char CTRL0;	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;     // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;    // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                                 // assert the SPI0 /SS strobe
  CTRL0  = txrx8bits(0xD2,1);                   // send op code (ignore returned data byte)
  CTRL0  = txrx8bits(0x00,1);                   // send dummy data / receive Status Reg byte           
   SPI0_nSS = 1;                                // negate the SPI0 /SS strobe
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;   // enable auto /SS output, set /SS Output Enable 	
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;    // enable auto /SS output, set SPI0 Mode Fault
  return CTRL0;
}

/* ------------------------------------------------------------------
/  Read HI-3110 Register Read Function
/  ------------------------------------------------------------------

Argument(s):  Register to read

     Return:  8-bit Register Value 
*/

unsigned char R_Register (char Reg) {                     
  unsigned char R_Reg;
	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;     // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;    // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                                 // assert the SPI0 /SS strobe
  R_Reg  = txrx8bits(Reg,1);                   // send op code (ignore returned data byte)
  R_Reg  = txrx8bits(0x00,1);                   // send dummy data / receive Status Reg byte           
   SPI0_nSS = 1;                                // negate the SPI0 /SS strobe
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;   // enable auto /SS output, set /SS Output Enable 	
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;    // enable auto /SS output, set SPI0 Mode Fault
  return R_Reg;
}


// ------------------------------------------------------
// Fetch the SPI FIFO data and load the data into the passed array pointer
// This is a more generic version of this function which could be used to read 
// other SPI multibyte reads.
// Inputs:  ReadCommand=SPI Command
//          count=number of bytes to read
//
// Output:  *passedArray = pointer to the array that gets loaded
// ------------------------------------------------------
void MultiByteRead(uint8 ReadCommand, uint8 count, unsigned char *passedArray)
{
   uint8 dummy, ByteCount;
   
   CS_HL();  
   dummy = SPI0SR;                  // clear SPI status register    
   SPI0DR = ReadCommand;            // SPI read command
   while (!SPI0SR_SPIF);            // wait for SPIF flag assertion    
   dummy = SPI0DR;                  // read/ignore Rx data in Data Reg, resets SPIF                    
   for (ByteCount=0; ByteCount < count; ByteCount++) {
      dummy = txrx8bits(0x00,1); 
      passedArray[ByteCount] = dummy; 
     }

   SPI0_nSS = 1;                                   // negate the SPI0 /SS strobe
   SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;          // enable auto /SS output, set /SS Output Enable 	
   SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;        // enable auto /SS output, set SPI0 Mode Fault
  
}


// ------------------------------------------------------
// Fetch the FIFO frame data and load the data into the passed array
// ------------------------------------------------------
void FetchFIFOFrame(unsigned char *passedArray)
{
  static uint8 dummy, ByteCount;
   
   CS_HL();  
   dummy = SPI0SR;         // clear SPI status register    
   SPI0DR = 0x46;          // SPI command Read Temp Message buffer with time tag
   while (!SPI0SR_SPIF);   // wait for SPIF flag assertion    
   dummy = SPI0DR;         // read/ignore Rx data in Data Reg, resets SPIF                    
   for (ByteCount=0; ByteCount<16; ByteCount++) {
      dummy = txrx8bits(0x00,1); 
      passedArray[ByteCount] = dummy; 
     }

   SPI0_nSS = 1;                                   // negate the SPI0 /SS strobe
   SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;          // enable auto /SS output, set /SS Output Enable 	
   SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;        // enable auto /SS output, set SPI0 Mode Fault
  
}

// ------------------------------------------------------------------
// Writes specified value into the Control-0 register of the 3110
// ------------------------------------------------------------------
void W_CTRL0 (char CTRL0) {
  unsigned char dummy;
  
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;    // disable auto /SS output, reset /SS Output Enable
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;  // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                              // assert the SPI0 /SS strobe
  dummy = SPI0SR;                            // clear SPI status register    
  SPI0DR = 0x14;                             // SPI Command    
  while (!SPI0SR_SPIF) {;}    
  dummy = SPI0DR;                            // read Rx data in Data Reg to reset SPIF
  //--------------------------------------------
  dummy = SPI0SR;                            // clear SPI status register    
  SPI0DR = CTRL0;                            // Pass on the command
  while (!SPI0SR_SPIF) {;}    
  dummy = SPI0DR;       // read Rx data in Data Reg to reset SPIF
  //--------------------------------------------

  SPI0_nSS = 1;                              // negate the SPI0 /SS strobe  	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;     // enable auto /SS output, set /SS Output Enable
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;   // enable auto /SS output, set SPI0 Mode Fault
  
} 

// ------------------------------------------------------------------
// Writes specified value into the Control-1 register of the 3110
// ------------------------------------------------------------------
void W_CTRL1 (char CTRL1) {
  unsigned char dummy;
  	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable 
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;  // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;  // assert the SPI0 /SS strobe
  dummy = SPI0SR;       // clear SPI status register    
  SPI0DR = 0x16;        // SPI Command    
  while (!SPI0SR_SPIF) {;}    
  dummy = SPI0DR;       // read Rx data in Data Reg to reset SPIF
  //--------------------------------------------
  dummy = SPI0SR;       // clear SPI status register    
    SPI0DR = CTRL1;      // Pass on the command
  while (!SPI0SR_SPIF) {;}    
  dummy = SPI0DR;       // read Rx data in Data Reg to reset SPIF
  //--------------------------------------------
  SPI0_nSS = 1;                           // negate the SPI0 /SS strobe	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;   // enable auto /SS output, set /SS Output Enable 
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
  
} 

// ------------------------------------------------------------------
// Read HI-3110 Status Flag Register Function
// ------------------------------------------------------------------
/*
Argument(s):  none

     Return:  8-bit Message Status Register
     
     Action:  Using SPI0, this function transmits op code 0xE2 plus
              1 byte of dummy data using two 8-bit SPI transfers.
              The received byte from SPI transfer #2 is returned  */
                   
  unsigned char STATF(void) {
  unsigned char STATF;
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;       // disable auto /SS output, reset /SS Output Enable 
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;      // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                                  // assert the SPI0 /SS strobe 

  STATF = txrx8bits(0xE2,1);                    // send op code (ignore returned data byte)  
  STATF = txrx8bits(0x00,1);                      // send dummy data / receive Status Reg byte   
  SPI0_nSS = 1;                                 // negate the SPI0 /SS strobe 	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;             // enable auto /SS output, set /SS Output Enable
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;           // enable auto /SS output, set SPI0 Mode Fault 
  return STATF;
  
}


/* ------------------------------------------------------------------
/  SPI0 8-Bit Send Data / Receive Data Function
/  ------------------------------------------------------------------

Argument(s):  txbyte, return_when_done

     Return:  rxbyte
     
     Action:  Using SPI0, this function sends txbyte and returns rxbyte
              as part of a chained multi-byte transfer. The calling 
              program controls the /SS chip select instead of using the
              automatic /SS option available in the Freescale SPI port.
              This permits simple chaining of op code commands and Tx/Rx
              data as a series of 8-bit read/writes followed by /SS 
              deassertion by the calling program.
              
              If return_when_done is True, the function waits for transfer
              completion before returning, which may be needed for back-to-
              back commands. If return_when_done is False, the function
              returns immediately after initiating the transfer. 
              
Example Call: rcv_byte = txrx8bits(0xFF,1) // sends data 0xFF then returns 
                                           // data when xfer is done  */
unsigned char txrx8bits (unsigned char txbyte, unsigned char return_when_done) {
  unsigned char rxbyte;  
  
  rxbyte = SPI0SR;         // clear SPI status register  
  SPI0DR = txbyte;         // write Data Register to begin transfer    
  if (return_when_done) {  // optional wait for SPIF flag
    while (!SPI0SR_SPIF);     
    }

  return rxbyte = SPI0DR;  // get received data byte from Data Register
  
 }
 
// ------------------------------------------------------------------
// Transmits the Frame data contained in the passed array pointer
// The first byte will be the 0x12 command byte followed 
// by the 12 bytes of frame data.
// ------------------------------------------------------------------
 void TransmitStandardFrame(uint8 *TXBuffer)
 {              
 uint8 static ByteCount,dummy;
 
   SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;         // disable auto /SS output, reset /SS Output Enable  
   SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;       // disable auto /SS output, reset SPI0 Mode Fault 
   SPI0_nSS = 0;                                   // assert the SPI0 /SS strobe
   for(ByteCount=0; ByteCount< 13; ByteCount++)    // Transmit command=0x12 +  12 bytes        
      {              
         dummy = txrx8bits(TXBuffer[ByteCount], 1);      // Transmit the whole message, ignore return values 
      }   
   SPI0_nSS = 1;                                         // negate the SPI0 /SS strobe        
   SPI0CR1 |= SPI0CR1_SPE_MASK|SPI0CR1_SSOE_MASK; 
   SPI0CR2 |= SPI0CR2_MODFEN_MASK;
         
}
 void TransmitExtendedFrame(uint8 *TXBuffer)
 {              
 uint8 static ByteCount,dummy;
 
   SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;         // disable auto /SS output, reset /SS Output Enable  
   SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;       // disable auto /SS output, reset SPI0 Mode Fault 
   SPI0_nSS = 0;                                   // assert the SPI0 /SS strobe    
   for(ByteCount=0; ByteCount< 15; ByteCount++)    // Transmit EXT Frame command=0x12 +  14 bytes    
      {              
         dummy = txrx8bits(TXBuffer[ByteCount], 1);      // Transmit the whole message, ignore return values 
      }   
   SPI0_nSS = 1;                                         // negate the SPI0 /SS strobe        
   SPI0CR1 |= SPI0CR1_SPE_MASK|SPI0CR1_SSOE_MASK; 
   SPI0CR2 |= SPI0CR2_MODFEN_MASK;
         
}

// ------------------------------------------------------------------
// Initialize the 3110, look for 0x82 in status register to verify.
// Pass = Don't turn on Red LED
// Fail = Turn On Red LED and Stop in dead loop
// ------------------------------------------------------------------
void INIT_3110(void){
    MASTER_RESET_SPI();  
    STAT_F = STATF();
    if (STAT_F  == STATFLAG_INIT)
      LED8 = OFF;  // Passed INIT
    else LED8 = ON;                         // Failed INIT
       while (!LED8);                       // STOP
}

// ------------------------------------------------------------------
// Master Reset
// ------------------------------------------------------------------
void MASTER_RESET_SPI (void) {
  static unsigned char dummy;
 	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable 
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;  // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                  // assert the SPI0 /SS strobe
  dummy = SPI0SR;               // clear SPI status register    
  SPI0DR = 0x56;                // Test Mode SPI Instruction - MASTER RESET CMD 
  while (!SPI0SR_SPIF) {;}      // Wait for data to come back in.
  dummy = SPI0DR;               // read Rx data in Data Reg to reset SPIF 
  //--------------------------------------------

  SPI0_nSS = 1;  // negate the SPI0 /SS strobe 	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;   // enable auto /SS output, set /SS Output Enable
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
}

// ------------------------------------------------------
// Set the SPI nCS high then low
// ------------------------------------------------------
void CS_HL (void) {
  SPI0_nSS = 1;                           // negate the SPI0 /SS strobe
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;  // enable auto /SS output, set /SS Output Enable
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault
  //--------------------------------------------    
	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;    // disable auto /SS output, reset /SS Output Enable 
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;  // disable auto /SS output, reset SPI0 Mode Fault
  SPI0_nSS = 0;                              // assert the SPI0 /SS strobe
  } 


   
// Load all the Filters then Masks contained in the Filters and Masks arrays
void WriteFiltersMasks(void)
{
  static unsigned char dummy;
  static uint8 i,set;
 		
   for(set=0; set < 8; set++)
   {	
     SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable
     SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK; // disable auto /SS output, reset SPI0 Mode Fault
     SPI0_nSS = 0;                             // assert the SPI0 /SS strobe

     dummy = SPI0SR;                           // clear SPI status register    
     for (i=0; i<7; i++) {          
         dummy = txrx8bits(Filters[set][i],1);
         }
         
     SPI0_nSS = 1;                             // negate the SPI0 /SS strobe
     SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;    // enable auto /SS output, set /SS Output Enable 	
     SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;  // enable auto /SS output, set SPI0 Mode Fault

  }
  
   for(set=0; set < 8; set++)
   {	
     SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;    // disable auto /SS output, reset /SS Output Enable
     SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;  // disable auto /SS output, reset SPI0 Mode Fault
     SPI0_nSS = 0;                              // assert the SPI0 /SS strobe

     dummy = SPI0SR;                            // clear SPI status register    
     for (i=0; i<7; i++) {          
         dummy = txrx8bits(Masks[set][i],1);
         }
         
     SPI0_nSS = 1;                              // negate the SPI0 /SS strobe
     SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;     // enable auto /SS output, set /SS Output Enable 	
     SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;   // enable auto /SS output, set SPI0 Mode Fault

  }  
}


// ------------------------------------------------------
// Read in the BITRATE0/1/2 switches and save it and initialize 3110.
// ------------------------------------------------------
void RATE_SELECT(void) {
  unsigned char dummy;
  
  RATE = PORTB & 0x07;       // get all bitrate switches   

 // First Set BTR0 Register	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;   // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK; // disable auto /SS output, reset SPI0 Mode Fault  
  SPI0_nSS = 0;             // assert the SPI0 /SS strobe
  
  dummy = SPI0SR;           // clear SPI status register    
  SPI0DR = 0x18;            // SPI Command  
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;           // read Rx data in Data Reg to reset SPIF    
  dummy = SPI0SR;           // clear SPI status register    
  SPI0DR = BitRate[RATE];   // Fetch rate per switch settings    
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;           // read Rx data in Data Reg to reset SPIF   
  SPI0_nSS = 1;                                    // negate the SPI0 /SS strobe 	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;         // enable auto /SS output, set /SS Output Enable
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;      // enable auto /SS output, set SPI0 Mode Fault

// Second Set BTR1 Register
  	
  SPI0CR1 = SPI0CR1 & ~SPI0CR1_SSOE_MASK;       // disable auto /SS output, reset /SS Output Enable  
  SPI0CR2 = SPI0CR2 & ~SPI0CR2_MODFEN_MASK;     // disable auto /SS output, reset SPI0 Mode Fault  
  SPI0_nSS = 0;                                 // assert the SPI0 /SS strobe
  dummy = SPI0SR;               // clear SPI status register    
  SPI0DR = 0x1A;                // SPI Command 
  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;               // read Rx data in Data Reg to reset SPIF
  dummy = SPI0SR;               // clear SPI status register    
  
#if EXTERNAL_TRANSCEIVER
   SPI0DR = 0x18;                // SAMP=0, TSEG1=9, TSEG2=2  83.3 % sample point for NXP TJA1040
#else
   SPI0DR = 0x27;                // SAMP=0, TSEG1=8, TSEG2=3  75% sample point for HI-3110
#endif

  while (!SPI0SR_SPIF);    
  dummy = SPI0DR;               // read Rx data in Data Reg to reset SPIF
  SPI0_nSS = 1;                 // negate the SPI0 /SS strobe  	
  SPI0CR1 = SPI0CR1 | SPI0CR1_SSOE_MASK;        // enable auto /SS output, set /SS Output Enable 
  SPI0CR2 = SPI0CR2 | SPI0CR2_MODFEN_MASK;       // enable auto /SS output, set SPI0 Mode Fault
   
}
// End

