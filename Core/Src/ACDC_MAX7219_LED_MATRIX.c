/**
 * @file ACDC_MAX7219_LED_MATRIX.c
 * @author Devin Marx
 * @brief Implementation of the external MAX7219 Serially Interfaced, 8-Digit LED Display Drivers
 * @version 0.1
 * @date 2024-04-14
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_MAX7219_LED_MATRIX.h"
#include "ACDC_SPI.h"

#define CALC_COL(col)               (((uint16_t)col + 1) << 8)  /** Calculates the desired column (For SPI Transmission) */
#define CALC_ROW(row)               (1 << row)                  /** Calculates the desired row (For SPI Transmission)    */
#define MAX_CLOCK_SPEED             10000000                    /** MAX1729 MAX Clock Freq = 10MHz {See MAX7219-1}       */

typedef enum {
    MAX_REG_NoOp        = 0x0000,   /**< MAX7219 Register No-Op               */
    MAX_REG_Digit0      = 0x0100,   /**< MAX7219 Register for Digit 0         */
    MAX_REG_Digit1      = 0x0200,   /**< MAX7219 Register for Digit 1         */
    MAX_REG_Digit2      = 0x0300,   /**< MAX7219 Register for Digit 2         */
    MAX_REG_Digit3      = 0x0400,   /**< MAX7219 Register for Digit 3         */
    MAX_REG_Digit4      = 0x0500,   /**< MAX7219 Register for Digit 4         */
    MAX_REG_Digit5      = 0x0600,   /**< MAX7219 Register for Digit 5         */
    MAX_REG_Digit6      = 0x0700,   /**< MAX7219 Register for Digit 6         */
    MAX_REG_Digit7      = 0x0800,   /**< MAX7219 Register for Digit 7         */
    MAX_REG_DecodeMode  = 0x0900,   /**< MAX7219 Register for the Decode Mode */
    MAX_REG_Intensity   = 0x0A00,   /**< MAX7219 Register for LED Intensity   */
    MAX_REG_ScanLimit   = 0x0B00,   /**< MAX7219 Register for the Scan Limit  */
    MAX_REG_Shutdown    = 0x0C00,   /**< MAX7219 Register for Shutdown/Enable */
    MAX_REG_DisplayTest = 0x0F00,   /**< MAX7219 Register for Display Testing */
} MAX7219_Registers;

MAX7219_t MaxLedMatrix_Init(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    SPI_InitCS(SPIx, true, GPIOx, GPIO_PIN);                            // Enable SPIx as master and enable the CS pins
    SPI_CalculateAndSetBaudDivider(SPIx, MAX_CLOCK_SPEED);              // 

    SPI_TransmitCS(SPIx, MAX_REG_Shutdown   | 0x01 , GPIOx, GPIO_PIN);  // Shutdown:    Normal Mode
    SPI_TransmitCS(SPIx, MAX_REG_Intensity  | 0x00 , GPIOx, GPIO_PIN);  // Intensity:   1/32 minimum brightness
    SPI_TransmitCS(SPIx, MAX_REG_ScanLimit  | 0x07 , GPIOx, GPIO_PIN);  // Scan Limit:  Display All Digits
    SPI_TransmitCS(SPIx, MAX_REG_DecodeMode | 0x00 , GPIOx, GPIO_PIN);  // Decode Mode: No decode for digits 7-0

    for(int i = 1; i < 9; i++)
        SPI_TransmitCS(SPIx, i << 8 | 0x00, GPIOx, GPIO_PIN);           // Clear the display (Set it all to 00's)

    return (MAX7219_t){SPIx, GPIOx, GPIO_PIN, {}};
}

void MaxLedMatrix_RedrawDisplay(MAX7219_t Display){
    for(uint8_t i = 0 ; i < 8; i++) // Iterate over each of the Columns
        SPI_TransmitCS(Display.SPIx, CALC_COL(i) | Display.DisplayBuffer[i], Display.GPIOx_CS, Display.GPIO_PIN_CS); // Redraw the column
}

void MaxLedMatrix_SetPixel(MAX7219_t *Display, uint8_t row, uint8_t col){
    if(Display == NULL || row >= 8 || col >= 8) // If the parameters are invalid
        return;

    Display->DisplayBuffer[col] |= CALC_ROW(row);   // Set the row bit in the column buffer
    SPI_TransmitCS(Display->SPIx, CALC_COL(col) | Display->DisplayBuffer[col], Display->GPIOx_CS, Display->GPIO_PIN_CS); // Display the updated Column
}

void MaxLedMatrix_ClearPixel(MAX7219_t *Display, uint8_t row, uint8_t col){
    if(Display == NULL || row >= 8 || col >= 8) // If the parameters are invalid
        return;

    Display->DisplayBuffer[col] &= ~CALC_ROW(row);  // Clear the row bit in the column buffer
    SPI_TransmitCS(Display->SPIx, CALC_COL(col) | Display->DisplayBuffer[col], Display->GPIOx_CS, Display->GPIO_PIN_CS); // Display the updated Column
}

bool MaxLedMatrix_LedStatus(MAX7219_t Display, uint8_t row, uint8_t col){
    if(row >= 8 || col >= 8) // If the parameters are invalid
        return false;

    return Display.DisplayBuffer[col] & CALC_ROW(row) ? true : false;   // If the row bit in the column is a 1 return true
}

void MaxLedMatrix_SetIntensity(MAX7219_t Display, MAX7219_DutyCycle dutyCycle){
    if(dutyCycle < MAX_DutyCycle_1_32 || dutyCycle > MAX_DutyCycle_31_32) // If the parameters are invalid
        return;

    SPI_TransmitCS(Display.SPIx, MAX_REG_Intensity | dutyCycle, Display.GPIOx_CS, Display.GPIO_PIN_CS); // Set the new LED Intensity
}