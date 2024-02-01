/**
 * @file ACDC_SPI.h
 * @author Liam Bramley
 * @brief Header file for SPI driver
 * @version 0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2023-2024
 * 
 */

#ifndef ACDC_SPI_H
#define ACDC_SPI_H

#include "stm32f1xx.h"

void SPI_Initalize(SPI_TypeDef *SPI);
void SPI_Transmit(uint16_t data, SPI_TypeDef *SPI);
uint16_t SPI_Receive(SPI_TypeDef *SPI);
uint16_t SPI_TransmitReceive(uint16_t data, SPI_TypeDef *SPI);

#endif