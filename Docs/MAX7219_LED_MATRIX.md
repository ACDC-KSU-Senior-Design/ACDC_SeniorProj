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

## Change the matrix data and redraw the display

```c
// SPI    => SPI2
// CS Pin => GPIOB, 1

int main(){
    /* Enable MCU clocks and other peripherals */
    MAX7219_t Display = MaxLedMatrix_Init(SPI2, GPIOB, GPIO_PIN_1); // Setup the Display, SPI2, and CS pin
    for(int i = 0; i < 8; i++)
        Display.DisplayBuffer[i] = i;       // Set the 0-7 column with 0-7
    MaxLedMatrix_RedrawDisplay(Display);    // Force redraw the displays
}
```