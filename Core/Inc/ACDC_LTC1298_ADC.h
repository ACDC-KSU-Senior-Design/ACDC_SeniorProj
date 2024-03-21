/**
 * @file ACDC_LTC1298IS8_ADC.h
 * @author Devin Marx
 * @brief Header file for the LTC1298 12-bit ADC
 * @version 0.1
 * @date 2024-02-21
 * 
 * @copyright Copyright (c) 2023-2024
 */
#ifndef __ACDC_LTC1298_ADC_H
#define __ACDC_LTC1298_ADC_H

#include "stm32f1xx.h"
#include "ACDC_stdint.h"

/// @brief Initiliazes SPIx and the external LTC1298IS8 ADC. Also sets up the software CS pin for SPIx
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param GPIOx GPIO Port for the chip select pin (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired chip select pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
void LTCADC_InitCS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/// @brief Reads the current ADC value on channel 0 (Software CS)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN_x Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @return 12-bits of data representing the ADC's output
uint16_t LTCADC_ReadCH0CS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x);

/// @brief Reads the current ADC value on channel 1 (SoftwareCS)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN_x Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @return 12-bits of data representing the ADC's output
uint16_t LTCADC_ReadCH1CS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x);


#endif