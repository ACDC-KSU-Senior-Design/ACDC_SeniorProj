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
#include "ACDC_GPIO.h"

/// @brief Initializes the SPI given
/// @param SPI SPI to initialize
void SPI_Initalize(const SPI_TypeDef *SPI);

/// @brief Transmits data over the given SPI
/// @param data Data to transmit
/// @param SPI SPI to transmit over
void SPI_Transmit(SPI_TypeDef *SPI, uint16_t data);

/// @brief Recieves data from a SPI
/// @param SPI SPI to recieve from
/// @return Data recieved
uint16_t SPI_Receive(const SPI_TypeDef *SPI);

/// @brief Transmits and recieves data over the SPI
/// @param data Data to transmit
/// @param SPI SPI to transmit over
/// @return Data recieved
uint16_t SPI_TransmitReceive(const SPI_TypeDef *SPI, uint16_t data);

#endif