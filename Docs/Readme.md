# Documentation

## Examples

* [ACDC_CLOCK.h](CLOCK.md)
  * Set and retrieve the System Clock Speed
  * Configure Prescalers for ADC, APB1, and APB2
  * Use MCU's MCO output (outputs the the HSE, HSI, SYSCLK, etc. on the MCO pin PA8)
* [ACDC_GPIO.h](GPIO.md)
  * Set GPIO to Input (Analog, Floating, Pulldown, Pullup)
  * Set GPIO to Output (Speed: 2Mhz, 10Mhz, 50Mhz and Push Pull or Open Drain)
  * Set, Clear, Toggle, and Write to Pins
  * Read Pins State
* [ACDC_INTERRUPT.h](INTERRUPT.md)
  * Set GPIO Pin to a Interrupt (Rising Edge, Falling Edge, Both Edges)
* [ACDC_LTC1298_ADC.h](LTC1298_ADC.md)
  * Read an analog voltage applied to either channel 0 or 1 on the ADC.
* [ACDC_LTC1451_ADC.h](LTC1451_DAC.md)
  * Set an analog voltage to the output of the LTC1451 DAC
* [ACDC_SPI.h](SPI.md)
  * Setup SPI as Master and transmit data in 8-bit or 16-bit modes
* [ACDC_TIMER.h](TIMER.md)
  * (SHOULD NOT BE CALLED BY USER) TIMER_Init & TIMER_SetSystemClockSpeed
  * Use Millis() to create a non blocking delay
  * Create a timed delay using the Delay function
* [ACDC_USART.h](USART.md)
  * Configure the UART/USART perpherial to use a multitude of baud rates from 1200bps - 230400bps.
  * Send and Recieve a single character or a whole string over UART/USART.
  * Change the Buad rate on the fly mid program and check for data in the USART buffer.
  