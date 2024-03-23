/**
 * @file ACDC_LTC1451_DAC.c
 * @author Devin Marx
 * @brief Implementation of the external 12-bit LTC1451 Digital to Analog Converter (DAC)
 * @version 0.1
 * @date 2024-03-21
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_LTC1451_DAC.h"
#include "ACDC_SPI.h"

#define MAX_CLOCK_SPEED 12500000UL  // Min timing CLK High = 40ns, CLK Low = 40ns (1/(40ns + 40ns)) = 12.5MHz {See LTC1451-4}

LTC1451_t LTCDAC_InitCS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    if(SPIx == SPI1)
       SPI_EnableRemap(SPIx, true);                        // Enable pin remapping on SPI1 so it has 5v tolerant pins
    SPI_InitCS(SPIx, true, GPIOx, GPIO_PIN);               // Enable SPIx as master and enable the CS pins
    SPI_CalculateAndSetBaudDivider(SPIx, MAX_CLOCK_SPEED); //TODO: Need to make a way to ASSERT if the CLOCK Speed is set low enough (BREAK IF NOT)
    return ((LTC1451_t){SPIx, GPIOx, GPIO_PIN});           // Return the newly initialized struct containing SPIx and its CS port/pin
}

void LTCDAC_SetOutputCS(LTC1451_t LTC_DAC, uint16_t outputVal){
    uint16_t dataToTransmit = outputVal & 0xFFF;    // Transmits MSBfirst and only the bottom 12 bits are used to set Vout {See LTC1451-8}
    SPI_TransmitCS(LTC_DAC.SPIx, dataToTransmit, LTC_DAC.GPIOx_CS, LTC_DAC.GPIO_PIN_CS);
}