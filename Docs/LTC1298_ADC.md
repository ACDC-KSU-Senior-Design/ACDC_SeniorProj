# ACDC_LTC1298_ADC.h

All functions below assume that you have included **"ACDC_LTC1298_ADC.h"**

## Setup the ADC on SPI2 and read a value from channel 0

```C
// SPI    => SPI2
// CS Pin => GPIOB, 12

int main(){
    /* Enable MCU clocks and other peripherals */
    LTCADC_InitCS(SPI2, GPIOB, GPIO_PIN_12);                                // Setup the ADC, SPI2, and CS pin
    uint16_t valueFromCH0 = LTCADC_ReadCH0CS(SPI2, GPIOB, GPIO_PIN_12);     // Read the data from the ADC
}
```

## Print the ADC data from channel 1 to the Serial Monitor

```C
// SPI    => SPI2
// CS Pin => GPIOB, 12

int main(void)
{
  /* Enable MCU clocks and other peripherals */
  USART_Init(USART2, Serial_115200, true);  // Initilize USART2 with a baud of 115200
  LTCADC_InitCS(SPI2, GPIOB, GPIO_PIN_12);  // Setup the ADC, SPI2, and CS pin

  while (1)
  {
    uint32_t adcData = LTCADC_ReadCH1CS(SPI2, GPIOB, GPIO_PIN_12); // Read the data from the ADC
    USART_SendString(USART2, StringConvert(adcData)); // Convert the number into a String and send it to the Serial Monitor
  }
}
```

## Print only unique data from the ADC to the Serial Monitor

```C
// SPI    => SPI2
// CS Pin => GPIOB, 12

int main(void)
{
  /* Enable MCU clocks and other peripherals */
  USART_Init(USART2, Serial_115200, true);  // Initilize USART2 with a baud of 115200
  LTCADC_InitCS(SPI2, GPIOB, GPIO_PIN_12);  // Setup the ADC, SPI2, and CS pin


  uint16_t oldData = 0; // Save the previous value
  while (1)
  {
    uint32_t newData = LTCADC_ReadCH0CS(SPI2, GPIOB, GPIO_PIN_12); // Read the data from the ADC channel 0
    if(newData != oldData){ // If the data is not the same (if the value changed)
      USART_SendString(USART2, StringConvert(newData)); // Send the updated value through the Serial monitor
      oldData = newData; // update the old value
    }
  }
}
```