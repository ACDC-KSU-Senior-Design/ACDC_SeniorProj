/**
 * @file ACDC_INTERRUPT.h
 * @author Devin Marx
 * @brief Contains interrupt functions for hardware peripherals
 * @version 0.1
 * @date 2023-11-02
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_INTERRUPT_H
#define __ACDC_INTERRUPT_H

#include "stm32f1xx.h"
#include "ACDC_stdint.h"

typedef enum{
    TT_RISING_EDGE             = 0b01, /**< Interrupt Occurs on Rising Edge                       */
    TT_FALLING_EDGE            = 0b10, /**< Interrupt Occurs on Falling Edge                      */
    TT_RISING_AND_FALLING_EDGE = 0b11  /**< Interrupt Occurs on both the Rising and Falling Edges */
} GPIO_TriggerType;

/// @brief Enables Interrupts for the specific vector IRQn
/// @param IRQn Interrupt vector to enable
void INTERRUPT_Enable(IRQn_Type IRQn);

/// @brief Disables Interrupts for the specific vector IRQn
/// @param IRQn Interrupt vector to disable
void INTERRUPT_Disable(IRQn_Type IRQn);

/// @brief Sets the priority of the Interrupt vector IRQn.
/// @param IRQn Interrupt vector
/// @param Priority Priority of the IRQn vector (Value: 0-15 Lower value means higher priority)
void INTERRUPT_SetPriority(IRQn_Type IRQn, uint8_t Priority);

/// @brief Enables Interrupts for GPIO_PIN on GPIOx. (Make sure to set GPIO_PIN as an Input)
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...) 
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @param triggerType Edge that triggers the Interrupt (Ex. RISING_EDGE, FALLING_EDGE, RISING_AND_FALLING_EDGE)
void GPIO_INT_SetToInterrupt(const GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, GPIO_TriggerType TT_x);

#endif