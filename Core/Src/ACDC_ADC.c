/**
 * @file ACDC_ADC.h
 * @author Liam Bramley
 * @brief Implementation of ADC driver
 * @version 0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2023-2024
 * 
 */

#include "ACDC_ADC.h"
#include "ACDC_SPI.h"
#include "ACDC_GPIO.h"
#include "ACDC_TIMER.h"

uint16_t ADC_Recieve(){
    GPIO_Set(GPIOC, GPIO_PIN_7);
    Delay(1);
    uint16_t data = SPI_Receive(SPI2);
    return data;
}

void ADC_Send(){
    GPIO_Clear(GPIOC, GPIO_PIN_7);
}