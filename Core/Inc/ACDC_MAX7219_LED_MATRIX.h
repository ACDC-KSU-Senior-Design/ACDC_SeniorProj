/**
 * @file ACDC_MAX7219_LED_MATRIX.h
 * @author Devin Marx
 * @brief Header file for the MAX7219 Serially Interfaced 8-Digit LED Displaay Driver
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_MAX7219_LED_MATRIX_H
#define __ACDC_MAX7219_LED_MATRIX_H

#include "stm32f1xx.h"
#include "ACDC_stdbool.h"

typedef struct{
    SPI_TypeDef *SPIx;          /**< SPI Peripheral to be used for the MAX7219 LED Driver */
    GPIO_TypeDef *GPIOx_CS;     /**< GPIO port for SPIx's CS                              */
    uint16_t GPIO_PIN_CS;       /**< PIO pin for SPI'x CS                                 */
    uint8_t DisplayBuffer[8];   /**< Buffer that holds the current state of the display   */
} MAX7219_t;

typedef enum{
    MAX_DutyCycle_1_32  = 0x00, /**< 1/32 Duty Cycle  */
    MAX_DutyCycle_3_32  = 0x01, /**< 3/32 Duty Cycle  */
    MAX_DutyCycle_5_32  = 0x02, /**< 5/32 Duty Cycle  */
    MAX_DutyCycle_7_32  = 0x03, /**< 7/32 Duty Cycle  */
    MAX_DutyCycle_9_32  = 0x04, /**< 9/32 Duty Cycle  */
    MAX_DutyCycle_11_32 = 0x05, /**< 11/32 Duty Cycle */
    MAX_DutyCycle_13_32 = 0x06, /**< 13/32 Duty Cycle */
    MAX_DutyCycle_15_32 = 0x07, /**< 15/32 Duty Cycle */
    MAX_DutyCycle_17_32 = 0x08, /**< 17/32 Duty Cycle */
    MAX_DutyCycle_19_32 = 0x09, /**< 19/32 Duty Cycle */
    MAX_DutyCycle_21_32 = 0x0A, /**< 21/32 Duty Cycle */
    MAX_DutyCycle_23_32 = 0x0B, /**< 23/32 Duty Cycle */
    MAX_DutyCycle_25_32 = 0x0C, /**< 25/32 Duty Cycle */
    MAX_DutyCycle_27_32 = 0x0D, /**< 27/32 Duty Cycle */
    MAX_DutyCycle_29_32 = 0x0E, /**< 29/32 Duty Cycle */
    MAX_DutyCycle_31_32 = 0x0F  /**< 31/32 Duty Cycle */
} MAX7219_DutyCycle;

/// @brief Initializes the MAX7219 Led Driver using SPIx and GPIOx/GPIO_PIN for the chip select.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param GPIOx GPIO Port for the chip select pin (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired chip select pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @return Struct containing all necessary data for the led matrix
MAX7219_t MaxLedMatrix_Init(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/// @brief Completely Redraws the Displays buffer.
/// @param Display Struct containing configuration data for the MAX7219 chip
void MaxLedMatrix_RedrawDisplay(MAX7219_t Display);

/// @brief Sets and displays the pixel located at (row,col).
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param row Desired Row of the display to set 
/// @param col Desired Column of the display to set
void MaxLedMatrix_SetPixel(MAX7219_t *Display, uint8_t row, uint8_t col);

/// @brief Clears and displays the pixel located at (row,col).
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param row Desired Row of the display to set 
/// @param col Desired Column of the display to set
void MaxLedMatrix_ClearPixel(MAX7219_t *Display, uint8_t row, uint8_t col);

/// @brief Checks if the LED at (row,col) is on or off.
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param row Desired Row of the display to set 
/// @param col Desired Column of the display to set
/// @return True if the led at (row,col) is turned on, false otherwise
bool MaxLedMatrix_LedStatus(MAX7219_t Display, uint8_t row, uint8_t col);

/// @brief Change the Intensityy (brightness) of the LED display.
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param dutyCycle Desired duty cycle (brightness) of the display (Ex. MAX_DutyCycle_1_32, MAX_DutyCycle_3_32, ...)
void MaxLedMatrix_SetIntensity(MAX7219_t Display, MAX7219_DutyCycle dutyCycle);

#endif