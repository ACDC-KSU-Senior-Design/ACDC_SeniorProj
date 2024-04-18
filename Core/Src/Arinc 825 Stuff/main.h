/*       Main.h
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

typedef unsigned char uint8;
typedef signed char int8;


#define FILTER0   0x62
#define MASK0     0x74

  
#define OFFF OFF
#define K_1MS  10
#define K_10MS 100
#define K_100MS  1000
#define K_1SEC  10000
#define TOGGLE 1

#define DEADLOOP 1

#define STANDARDFRAME   1
#define EXTENDEDFRAME   2


#define TEST_MODE        0
#define TRANSMIT_MODE    1
#define RECEIVE_MODE     2

#define MONITOR_MODE     3
#define LOOPBACK_MODE    4

#define TRANSMITEXT_MODE 5
#define RECEIVER_INTERRUPT_MODE 6
#define SERIAL_COMMANDS 7

#define OPT1       1            // SW2 position 1
#define OPT2       2            // SW2 position 2

#define PP0Interrupt 1
#define PP1Interrupt 2
#define PP2Interrupt 4
#define PP3Interrupt 8

// Prototypes


#pragma CODE_SEG NON_BANKED
interrupt 8 void TIMER_ISR(void);

#pragma CODE_SEG NON_BANKED
interrupt 56 void PORTP_ISR(void);

#pragma CODE_SEG DEFAULT
void Delay100us(int delay);
void PeriphInit(void);
void Timer_Init(void);
void PortP_Init(void);
void MODE_SELECT(void);
void TEST_OPTION(void);
void Sleep_Mode(void);
void Receive_Mode(void);
void Monitor_Mode(void);
void LoopBack_Mode(void);
void Transmit_Mode(uint8 frameType);
void Receiver_Interrupt_Mode(void);
void GPIO_InitInterrupts(void);
void HW_RESET(void);
void PackPayLoad8(unsigned char frameType, unsigned char *array);
void W_GPINE (unsigned char stat);
void getRegStatus(void);
void LED_CTL(uint8 ledNumber, uint8 OnOff);
void GPINE(uint8 GPINE2, uint8 GPINE1 );
void SerialCommands(void);
void getErrorStatus(void);
void debounceButton(uint8 button);
void printCANDataIndex(unsigned char *array, unsigned char index);
void printCANFrame(unsigned char MessageCount);
void printCANDataIndexSpecial(unsigned char *array, unsigned char index);


