# ACDC_LTC1451_ADC.h

All functions below assume that you have included **"ACDC_LTC1451_DAC.h"**

## Setup the DAC on SPI2 and set the output to 1v

```c
// SPI    => SPI2
// CS Pin => GPIOC, 4

int main(){
    /* Enable MCU clocks and other peripherals */
    LTCDAC_InitCS(SPI2, GPIOC, GPIO_PIN_4);             // Setup the DAC, SPI2, and CS pin
    LTCDAC_SetOutputCS(SPI2, GPIOC, GPIO_PIN_4, 1000);  // Set the DAC output to 1000/4095 or 1v
}
```