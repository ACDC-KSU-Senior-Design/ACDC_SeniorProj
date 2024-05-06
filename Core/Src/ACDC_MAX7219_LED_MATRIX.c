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
#include "ACDC_GPIO.h"
#include "ACDC_SPI.h"

#define NUM_LED_IN_COL              (8)
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

#pragma region PRIVATE_FUNCTION_PROTOTYPES
/// @brief Sends a SPI message to the entire display (Sends the same message to each display)
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param Message SPI instruction to send to each of the displays
static void MaxLedMatrix_SendSpiCommand(MAX7219_t Display, uint16_t Message);

/// @brief Redraws the column col on each attached display
/// @param Display Struct containing configuration data for the MAX7219 chip
/// @param column Column to redraw on each display
static void MaxLedMatrix_UpdateColumn(MAX7219_t Display, uint8_t col);
#pragma endregion

MAX7219_t MaxLedMatrix_Init(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t numDisplays, uint8_t *displayMem){
    for(uint8_t i = 0; i < numDisplays * 8; i++)                        // Iterate over each index of the displays memory
        displayMem[i] = 0;                                              // Erase the contents of the memory

    SPI_InitCS(SPIx, true, GPIOx, GPIO_PIN);                            // Enable SPIx as master and enable the CS pins
    SPI_CalculateAndSetBaudDivider(SPIx, MAX_CLOCK_SPEED);              
    MAX7219_t Display = {SPIx, GPIOx, GPIO_PIN, numDisplays, displayMem};

    MaxLedMatrix_SendSpiCommand(Display, MAX_REG_Shutdown   | 0x01);    // Shutdown:    Normal Mode
    MaxLedMatrix_SendSpiCommand(Display, MAX_REG_Intensity  | 0x00);    // Intensity:   1/32 (minimum brightness) 
    MaxLedMatrix_SendSpiCommand(Display, MAX_REG_ScanLimit  | 0x07);    // Scan Limit:  Display All Digits
    MaxLedMatrix_SendSpiCommand(Display, MAX_REG_DecodeMode | 0x00);    // Decode Mode: No decode for digits 7-0
    MaxLedMatrix_RedrawDisplay(Display);                                // Redraw the display with all 0's

    return Display;
}

void MaxLedMatrix_RedrawDisplay(MAX7219_t Display){
    for(uint8_t i = 0 ; i < 8; i++){                                                    // Iterate over each of the Columns
        GPIO_Clear(Display.GPIOx_CS, Display.GPIO_PIN_CS);                              // Set the CS pin low until data is sent to each display
        for(int8_t j = Display.NumberOfDisplays-1; j >= 0; j--){                        // Iterate over each display
            uint8_t column = j*8 + i;                                                   // Calculate the index of the columns data
            SPI_Transmit(Display.SPIx, CALC_COL(i) | Display.DisplayBuffer[column]);    // Transmit the updated column data to the display
        }
        while(Display.SPIx->SR & SPI_SR_BSY){}                                          // Wait until the SPI Tx has completed
        GPIO_Set(Display.GPIOx_CS, Display.GPIO_PIN_CS);                                // Set the CS pin high
    }
}

void MaxLedMatrix_SetPixel(MAX7219_t *Display, uint8_t row, uint8_t col){
    if(Display == NULL || row >= 8 || col >= Display->NumberOfDisplays*8) // If the parameters are invalid
        return;

    Display->DisplayBuffer[col] |= CALC_ROW(row);   // Set the row bit in the column buffer
    MaxLedMatrix_UpdateColumn(*Display, col);       // Display the updated Column
}

void MaxLedMatrix_ClearPixel(MAX7219_t *Display, uint8_t row, uint8_t col){
    if(Display == NULL || row >= 8 || col >= Display->NumberOfDisplays*8) // If the parameters are invalid
        return;

    Display->DisplayBuffer[col] &= ~CALC_ROW(row);  // Clear the row bit in the column buffer
    MaxLedMatrix_UpdateColumn(*Display, col);       // Display the updated Column
}

bool MaxLedMatrix_LedStatus(MAX7219_t Display, uint8_t row, uint8_t col){
    if(row >= 8 || col >= Display.NumberOfDisplays*8) // If the parameters are invalid
        return false;

    return (Display.DisplayBuffer[col] & CALC_ROW(row)) ? true : false;   // If the row bit in the column is a 1 return true
}

void MaxLedMatrix_SetIntensity(MAX7219_t Display, MAX7219_DutyCycle dutyCycle){
    if(dutyCycle < MAX_DutyCycle_1_32 || dutyCycle > MAX_DutyCycle_31_32)   // If the parameters are invalid
        return;

    MaxLedMatrix_SendSpiCommand(Display, MAX_REG_Intensity | dutyCycle);    // Set the new LED Intensity
}

#pragma region PRIVATE_FUNCTIONS
static void MaxLedMatrix_SendSpiCommand(MAX7219_t Display, uint16_t Message){
    GPIO_Clear(Display.GPIOx_CS, Display.GPIO_PIN_CS);      // Set the CS pin low
    for(uint8_t i = 0; i < Display.NumberOfDisplays; i++)   // Iterate over each display
        SPI_Transmit(Display.SPIx, Message);                // Send the same message to each display
    while(READ_BIT(Display.SPIx->SR, SPI_SR_BSY)){}         // Waits until the SPI message has been sent
    GPIO_Set(Display.GPIOx_CS, Display.GPIO_PIN_CS);        // Set the CS pin high
}

static void MaxLedMatrix_UpdateColumn(MAX7219_t Display, uint8_t col){
    col %= NUM_LED_IN_COL;                                                          // Ensure the column  is between 0 and 7
    GPIO_Clear(Display.GPIOx_CS, Display.GPIO_PIN_CS);                              // Set the CS pin low
    for(int8_t i = Display.NumberOfDisplays-1; i>=0; i--){                          // Iterate over each display
        uint8_t column = i*8 + col;                                                 // Calculate the index of the columns data
        SPI_Transmit(Display.SPIx, CALC_COL(col) | Display.DisplayBuffer[column]);  // Display the updated Column
    }
    while(READ_BIT(Display.SPIx->SR, SPI_SR_BSY)){}                                 // Wait until the SPI message has been sent
    GPIO_Set(Display.GPIOx_CS, Display.GPIO_PIN_CS);                                // Set the CS pin high to finish the transmission
}
#pragma endregion