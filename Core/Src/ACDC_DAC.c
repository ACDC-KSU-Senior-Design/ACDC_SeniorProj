/**
 * @file ACDC_ADC.h
 * @author Liam Bramley
 * @brief Implementation of DAC driver
 * @version 0.1
 * @date 2024-02-27
 * @copyright Copyright (c) 2023-2024
 * 
 */

#include "ACDC_DAC.h"
#include "ACDC_SPI.h"
#include "ACDC_GPIO.h"
#include "ACDC_TIMER.h"

uint16_t DAC_Recieve(){
    GPIO_Set(GPIOA, GPIO_PIN_8);
    Delay(1);
    uint16_t data = SPI_Receive(SPI2);
    return data;
}

void DAC_Send(){
    GPIO_Clear(GPIOA, GPIO_PIN_8);
}