/**
 * @file ACDC_ADC.h
 * @author Liam Bramley
 * @brief Header file for DAC driver
 * @version 0.1
 * @date 2024-02-27
 * @copyright Copyright (c) 2023-2024
 * 
 */

#include "stm32f1xx.h"

/// @brief Recieves data from the DAC
/// @return Returns the data recieved in the form of a uint16_t
uint16_t DAC_Recieve();

/// @brief Sets the DAC's chip select so it performs a conversion from the data hardwired into it
void DAC_Send();