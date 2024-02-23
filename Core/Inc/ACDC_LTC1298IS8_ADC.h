/**
 * @file ACDC_LTC1298IS8_ADC.h
 * @author Devin Marx
 * @brief 
 * @version 0.1
 * @date 2024-02-21
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef __ACDC_LTC1298IS8_ADC_H
#define __ACDC_LTC1298IS8_ADC_H

#include "stm32f1xx.h"
#include "ACDC_stdint.h"

/// @brief Initiliazes the external LTC1298IS8 ADC to work with SPIx
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
void LTCADC_Init(SPI_TypeDef *SPIx);

/// @brief Reads the current ADC value on channel 0
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @return 12-bits of data representing the ADC's output
uint16_t LTCADC_ReadCH0(SPI_TypeDef *SPIx);

/// @brief Reads the current ADC value on channel 1
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @return 12-bits of data representing the ADC's output
uint16_t LTCADC_ReadCH1(SPI_TypeDef *SPIx);

#endif