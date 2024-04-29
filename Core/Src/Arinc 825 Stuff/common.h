/*       common.h
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
 
#define ON 0
#define OFF 1
#define YES 1
#define NO 0
#define HI 1
#define LO 0
#define TRUE 1
#define FALSE 0


/* define value for switches when up (not pressed) and down (pressed) */
#define UP 1
#define DOWN 0

// PORTA
#define OPTION_0    PORTA_PA0            // Caution: Marked as OPT1 on the board
#define OPTION_1    PORTA_PA1            // Caution: Marked as OPT2 on the board
#define MR          PORTA_PA2            // Output Master Reset
#define TXEN        PORTA_PA3            // Output Transmit Enable Via Pin
#define MODE0       PORTA_PA4            // Input 
#define MODE1       PORTA_PA5            // Input 
#define MODE2       PORTA_PA6            // Input 
#define PA7         PORTA_PA7            // Spare

#define PORTP       PTP                  // PORTP friendly label
#define PORTT       PTT                  // PORTT friendly label

// PORTB
#define BITRATE0    PORTB_PB0            // Input 
#define BITRATE1    PORTB_PB1            // Input 
#define BITRATE2    PORTB_PB2            // Input 
#define PB3         PORTB_PB3            // Spare
#define SW1         PORTB_PB4            // Input Pushbutton Switch 
#define SW2         PORTB_PB5            // Input Pushbutton Switch
#define SW3         PORTB_PB6            // Input Pushbutton Switch
#define SW4         PORTB_PB7            // Input Pushbutton Switch

#define BUTTON1   0x10                   // Pushbutton switch debouncing identifiers
#define BUTTON2   0x20
#define BUTTON3   0x40
#define BUTTON4   0x80

// PORTE
#define LED5        PORTE_PE4            // Output, low=On
#define LED6        PORTE_PE5            // Output
#define LED7        PORTE_PE6            // Output
#define LED8        PORTE_PE7            // Output


// PORTT
// access full port byte with label:"PTT or PORTT"
#define LED1        PTT_PTT0             // Output, low=On
#define LED2        PTT_PTT1             // Output
#define LED3        PTT_PTT2             // Output
#define LED4        PTT_PTT3             // Output

// Labels for controlling the LED1-LED8
#define LED_1 1
#define LED_2 2
#define LED_3 3
#define LED_4 4
#define LED_5 5
#define LED_6 6
#define LED_7 7
#define LED_8 8
 
 
 