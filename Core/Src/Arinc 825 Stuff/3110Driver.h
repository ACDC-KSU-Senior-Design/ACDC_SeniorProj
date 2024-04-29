/*       3110Driver.h
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
#include "common.h"


// define 3110 Input Pins Interrupt on PORTP
// access full port with label "PTP" or "PORTP"
#define INT         DDRP_DDRP0            // Input
#define STAT        DDRP_DDRP1            // Input
#define GP2         DDRP_DDRP2            // Input
#define GP1         DDRP_DDRP3            // Input

// define chip select output for SPI port used to control pin directly 
#define SPI0_nSS    PTM_PTM3    


// HI-3110 SPI Register defines
// Write Registers
#define FIFO_W              0x12                // Write FiFi reg
#define CTR0_W              0x14                // Control Reg 0 Write
#define CTR1_W              0x16                // Control Reg 1 Write
#define BTR0_W              0x18                // Bit Rate 0 Reg Write
#define BTR1_W              0x1A                // Bit Rate 1 Reg Write
#define TEC_W               0x26                // TEC Reg Write
#define REC_W               0x24                // REC Reg Write
#define INTE_W              0x1C                // Interrupt Enable Reg Write
#define STATFE_W            0x1E                // Status Flag Enable Reg Write
#define GPINE_W             0x22                // Gen Purpose Pins Enable Reg Write

// Read Registers
#define CTR0_R              0xD2                // Control Reg 0 Read
#define CTR1_R              0xD4                // Control Reg 1 Read
#define BTR0_R              0xD6                // Bit Rate 0 Reg Read
#define BTR1_R              0xD8                // Bit Rate 1 Reg Read
#define TEC_R               0xEC                // TEC Reg Read
#define REC_R               0xEA                // REC Reg Read
#define MESSTAT_R           0xDA                // Message Status Reg Read
#define ERR_R               0xDC                // Error Reg Read
#define INTF_R              0xDE                // Interrupt Flag Reg Read
#define INTE_R              0xE4                // Interrupt Enable Reg Read
#define STATF_R             0xE2                // Status Flag Reg Read (82H after reset)
#define STATFE_R            0xE6                // Status Flag Enable Reg Read
#define GPINE_R             0xE8                // Gen Purpose Pins Enable Reg Read
#define TXFIFOHISTORY_R     0xEE            // Transmit FIFO History
#define TIMERUB_R           0xFA                // Free-running Timer Upper/Lower Byte Read

// 3110 Commands

#define TEMP_TAG_R              0X42    // Read temp register with tag 15 bytes
#define TEMP_NO_TAG_R           0x44    // Read temp register with no tag 15 bytes
#define FIFO_TAG_R              0x46    // Read FIFO with Time Tag
#define FIFO_NO_TAG_R           0x48    // Read FIFO no Tag 14 bytes
#define NEXT_FIFO_MESG_TAG_R    0x4A    // Read next FIFO with tag 11 bytes
#define NEXT_FIFO_NO_TAG_R      0x4C    // Read next FIFO No Tag 9 bytes


// INTERRUPT ENABLE INT PIN ENABLE DEFINES
// Setting these bits in the INTE register will cause the INT pin to be asserted
// when that event occurs.
#define RXTMPIE                 0X80
#define RXFIFOIE                0X40
#define TXCPLTIE                0X20
#define BUSERRIE                0X10
#define MCHGIE                  0X08
#define WAKEUPIE                0X04
#define F1MESSIE                0X02
#define F0MESSIE                0X01


// STATUS FLAG PIN ENABLE REGISTER DEFINES
#define TXMTYIE                 0X80
#define TXFULLIE                0X40
#define TXHISFIE                0X20
#define ERRWIE                  0X10
#define ERRPIE                  0X08
#define BUSOFFIE                0X04
#define RXFMTYIE                0X02
#define RXFFULLIE               0X01

// Status Flag registers
#define TXMTY                    0x80
#define TXFULL                   0x40  
#define TXHISF                   0x20
#define ERRW                     0x10
#define ERRP                     0x08
#define BUSOFF                   0x04
#define RXFMTY                   0x02      
#define RXFFULL                  0x01  

// INTF Flag register bits
#define RXTMP                    0x80
#define RXFIFO                   0x40  
#define TXCPLT                   0x20
#define BUSERR                   0x10
#define MCHG                     0x08
#define WAKEUP                   0x04
#define F1MESS                   0x02      
#define F0MESS                   0x01 

// GPINE INTF registers
#define GPIN2_RXFIFO  0x6
#define GPIN1_TXCPLT  0x5
 

// GPINE registers
#define GPIN_RXFFULL             0x08 
#define GPIN_RXFMPTY             0x09
#define GPIN_BUSOFF              0x0A
#define GPIN_ERRP                0x0B

#define GPIN_ERRW                0x0C
#define GPIN_TXHISF              0x0D 
#define GPIN_TXFFULL             0x0E
#define GPIN_TXMPTY              0x0F
 
#define R_Filter0               0xA2
#define R_Filter1               0xA4
#define R_Filter2               0xA6
#define R_Filter3               0xA8
#define R_Filter4               0xAA
#define R_Filter5               0xAC
#define R_Filter6               0xAE
#define R_Filter7               0xB2



#define R_Mask0                 0xB4
#define R_Mask1                 0xB6
#define R_Mask2                 0xB8
#define R_Mask3                 0xBA
#define R_Mask4                 0xBC
#define R_Mask5                 0xBE
#define R_Mask6                 0xC2
#define R_Mask7                 0xC4

#define W_FILTER0               0x62
#define W_FILTER1               0x64
#define W_FILTER2               0x66
#define W_FILTER3               0x68
#define W_FILTER4               0x6A
#define W_FILTER5               0x6C
#define W_FILTER6               0x6E
#define W_FILTER7               0x72

#define W_MASK0                 0x74
#define W_MASK1                 0x76
#define W_MASK2                 0x78
#define W_MASK3                 0x7A
#define W_MASK4                 0x7C
#define W_MASK5                 0x7E
#define W_MASK6                 0x82
#define W_MASK7                 0x84

// define HI-3110 SPI command *
// 3110 MODES
#define NORMAL 0              
#define LOOPBACK 1
#define MONITOR 2
#define SLEEP 3
#define INIT 4
#define TTDIV0 0
#define TTDIV2 1
#define TTDIV4 2
#define TTDIV8 3
#define WAKEUP_ON 1
#define WAKEUP_OFF 0
#define RESET_YES 1
#define RESET_NO 0
#define BOR_ON 1
#define BOR_OFF 0

// Status of Modes in CTRL0 register
#define NORMAL_STATUS      0
#define LOOPBACK_STATUS    0x20 
#define MONITOR_STATUS     0x40
#define SLEEP_STATUS       0x60
#define INIT_STATUS        0x80


#define STATFLAG_INIT  0x82              // Default values after Master Reset

#define RDY 0x82
#define IDE_BIT 0x80

void INIT_3110(void);
void MASTER_RESET_SPI(void);
void RATE_SELECT(void);  
unsigned char STATF (void);
unsigned char txrx8bits (unsigned char , unsigned char );
unsigned char R_CTRL0 (void);
void CS_HL (void);
void W_STATFE (unsigned char);
unsigned char txrx8bits (unsigned char, unsigned char );
void W_CTRL0 (char CTRL0);
void W_CTRL1 (char CTRL1);
unsigned char ReadReg(char cmd);
uint8 Init3110(const mode, uint8 ttdiv, uint8 wakeup, uint8 reset, uint8 bor);
unsigned char fetch8bits (unsigned char txbyte, unsigned char return_when_done);
unsigned char R_Register (char Reg);
void W_CommandValue (uint8 cmd, uint8 value);

void WriteFiltersMasks(void);
void TransmitStandardFrame(uint8 *TXBuffer);
void TransmitExtendedFrame(uint8 *TXBuffer);
void MultiByteRead(uint8,uint8 count, unsigned char *passedArray);
void FetchFIFOFrame(unsigned char *passedArray);
uint8 getNormalStatus(void);
uint8 getMonitorStatus(void);
uint8 getSleepStatus(void);

 