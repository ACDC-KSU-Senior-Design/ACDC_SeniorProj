# ACDC_SPI.h

All functions below assume that you have includeded **"ACDC_SPI.h"**

## Initialize SPI1 in 16-bit mode at 36MHz and Transmit data every second

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"
#include "ACDC_GPIO.h"
#include "ACDC_SPI.h"

/** SPI1 Info
 * CLK:  GPIOA, GPIO_PIN_5
 * MISO: GPIOA, GPIO_PIN_6
 * MOSI: GPIOA, GPIO_PIN_7
 * CS:   GPIOA, GPIO_PIN_8
 */

const uint16_t randomData = 0b1010101010101010;

int main(){
    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (CALLS TIMER_Init)
    GPIO_PinDirection(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_PUSH_PULL);  // Set the CS
    SPI_Init(SPI1);

    while(1){
        Delay(1000);                            // Wait 1000 milliseconds
        GPIO_Clear(GPIOA, GPIO_PIN_8);          // Set CS low before transmitting
        SPI_Transmit(SPI1, randomData);         // Transmit the random data 
        while(READ_BIT(SPIx->SR, SPI_SR_BSY)){} // Wait until the transfer has completed
        GPIO_Set(GPIOA, GPIO_PIN_8);            // Set the CS pin to high
    }
}
```
