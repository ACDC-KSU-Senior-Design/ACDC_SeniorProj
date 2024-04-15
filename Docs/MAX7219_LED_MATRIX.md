# ACDC_MAX7219_LED_MATRIX.h

All functions below assume that you have included **"ACDC_MAX7219_LED_MATRIX.h"**

[MAX7219 Datasheet](https://www.sparkfun.com/datasheets/Components/General/COM-09622-MAX7219-MAX7221.pdf)

## Setup the LED Matrix on SPI2 and set a pixel in the top left corner

```c
// SPI    => SPI2
// CS Pin => GPIOB, 1

int main(){
    /* Enable MCU clocks and other peripherals */
    MAX7219_t Display = MaxLedMatrix_Init(SPI2, GPIOB, GPIO_PIN_1); // Setup the Display, SPI2, and CS pin
    MaxLedMatrix_SetPixel(&Display, 0,0);                           // Set the pixel in the top left
}
```

## Display a grid on the matrix with a brightness of 15/32

```c
// SPI    => SPI2
// CS Pin => GPIOB, 1

int main(){
    /* Enable MCU clocks and other peripherals */
    MAX7219_t Display = MaxLedMatrix_Init(SPI2, GPIOB, GPIO_PIN_1);
    MaxLedMatrix_SetIntensity(Display, MAX_DutyCycle_15_32);
    Display.DisplayBuffer[0] = 0xAA;
    Display.DisplayBuffer[1] = 0x55;
    Display.DisplayBuffer[2] = 0xAA;
    Display.DisplayBuffer[3] = 0x55;
    Display.DisplayBuffer[4] = 0xAA;
    Display.DisplayBuffer[5] = 0x55;
    Display.DisplayBuffer[6] = 0xAA;
    Display.DisplayBuffer[7] = 0x55;
    MaxLedMatrix_RedrawDisplay(Display);
}
```