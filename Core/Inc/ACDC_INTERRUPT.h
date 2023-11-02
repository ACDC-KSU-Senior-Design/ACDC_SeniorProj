#ifndef __ACDC_INTERRUPT_H
#define __ACDC_INTERRUPT_H

#include "stm32f103xb.h"
#include "ACDC_stdint.h"

typedef enum{
    RISING_EDGE             = 0b01,
    FALLING_EDGE            = 0b10,
    RISING_AND_FALLING_EDGE = 0b11
} GPIO_TriggerType;

/// @brief Enables Interrupts for GPIO_PIN on GPIOx. (Make sure to set GPIO_PIN as an Input)
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...) 
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @param triggerType Edge that triggers the Interrupt (Ex. RISING_EDGE, FALLING_EDGE, RISING_AND_FALLING_EDGE)
void GPIO_INT_SetToInterrupt(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, GPIO_TriggerType triggerType);

#endif