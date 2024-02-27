/**
 * @file ACDC_ADC.h
 * @author Liam Bramley
 * @brief Header file for ADC driver
 * @version 0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2023-2024
 * 
 */

#include "stm32f1xx.h"

/// @brief Recieves data from the ADC
/// @return Returns the data recieved in the form of a uint16_t
uint16_t ADC_Recieve();

/// @brief Sets the ADC's chip select so it performs a conversion from the data hardwired into it
void ADC_Send();