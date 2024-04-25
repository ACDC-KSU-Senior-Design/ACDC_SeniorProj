/**
 * @file ACDC_TIMER.h
 * @author Devin Marx
 * @brief Header file containing functions for system timing using a hardware timer.
 * 
 * This file defines functions for initializing a system clock timer, setting its value, 
 * and providing timing-related functionalities such as retrieving number of milliseconds
 * since startup and introducing delays in code execution.
 * 
 * @version 0.1
 * @date 2023-11-7
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_TIMER_H
#define __ACDC_TIMER_H

#include "ACDC_CLOCK.h"
#include "ACDC_stdint.h"
#include "ACDC_stdbool.h"

typedef struct {
    TIM_TypeDef *TIMx;      /**< Pointer to the TIMx peripheral */
    uint8_t TimerChannel;   /**< Timer channel number */
    GPIO_TypeDef *GPIOx;    /**< GPIO port for TIMx and TimerChannel */
    uint16_t GPIO_PIN_x;    /**< GPIO pin for TIMx and TimerChannel*/
    bool isRM;              /**< True this is a remapped pin, false if it is the defualt pin function */
} TIMx_CHx;

typedef enum {
    PWM_MODE_1 = 0b01100000,    /**< Each PWM Cycle looks like this: ▔▔▔┃▂▂▂┃ (Starts High then transitions to Low)*/
    PWM_MODE_2 = 0b01110000     /**< Each PWM Cycle looks like this: ▂▂▂┃▔▔▔┃ (Starts Low then transitions to High)*/
} PWM_MODE;

// Does not include Alternate function 
#define TIM1_CH1_PB13 ((TIMx_CHx){TIM1, 1, GPIOB, GPIO_PIN_13, false})     // Timer 1 Channel 1 on GPIOB Pin 13
#define TIM1_CH2_PB14 ((TIMx_CHx){TIM1, 2, GPIOB, GPIO_PIN_14, false})     // Timer 1 Channel 2 on GPIOB Pin 14
#define TIM1_CH3_PA10 ((TIMx_CHx){TIM1, 3, GPIOA, GPIO_PIN_10, false})     // Timer 1 Channel 3 on GPIOA Pin 10
#define TIM1_CH4_PA11 ((TIMx_CHx){TIM1, 4, GPIOA, GPIO_PIN_11, false})     // Timer 1 Channel 4 on GPIOA Pin 11

#define TIM2_CH1_PA0 ((TIMx_CHx){TIM2, 1, GPIOA, GPIO_PIN_0, false})       // Timer 2 Channel 1 on GPIOA Pin 0
#define TIM2_CH2_PA1 ((TIMx_CHx){TIM2, 2, GPIOA, GPIO_PIN_1, false})       // Timer 2 Channel 2 on GPIOA Pin 1
#define TIM2_CH3_PA2 ((TIMx_CHx){TIM2, 3, GPIOA, GPIO_PIN_2, false})       // Timer 2 Channel 3 on GPIOA Pin 2
#define TIM2_CH4_PA3 ((TIMx_CHx){TIM2, 4, GPIOA, GPIO_PIN_3, false})       // Timer 2 Channel 4 on GPIOA Pin 3
 
#define TIM3_CH1_PA6 ((TIMx_CHx){TIM3, 1, GPIOA, GPIO_PIN_6, false})       // Timer 3 Channel 1 on GPIOA Pin 6
#define TIM3_CH2_PA7 ((TIMx_CHx){TIM3, 2, GPIOA, GPIO_PIN_7, false})       // Timer 3 Channel 2 on GPIOA Pin 7
#define TIM3_CH3_PB0 ((TIMx_CHx){TIM3, 3, GPIOB, GPIO_PIN_0, false})       // Timer 3 Channel 3 on GPIOB Pin 0
#define TIM3_CH4_PB1 ((TIMx_CHx){TIM3, 4, GPIOB, GPIO_PIN_1, false})       // Timer 3 Channel 4 on GPIOB Pin 1

#define TIM4_CH1_PB6 ((TIMx_CHx){TIM4, 1, GPIOB, GPIO_PIN_6, false})       // Timer 4 Channel 1 on GPIOB Pin 6
#define TIM4_CH2_PB7 ((TIMx_CHx){TIM4, 2, GPIOB, GPIO_PIN_7, false})       // Timer 4 Channel 2 on GPIOB Pin 7
#define TIM4_CH3_PB8 ((TIMx_CHx){TIM4, 3, GPIOB, GPIO_PIN_8, false})       // Timer 4 Channel 3 on GPIOB Pin 8
#define TIM4_CH4_PB9 ((TIMx_CHx){TIM4, 4, GPIOB, GPIO_PIN_9, false})       // Timer 4 Channel 4 on GPIOB Pin 9


/// @brief (SHOULD NOT BE CALLED BY USER, CALLED IN ACDC_CLOCK.h) Initilizes the SysClock timer and sets the value of SysClock->LOAD
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void TIMER_Init(SystemClockSpeed SCS_x);

/// @brief (SHOULD NOT BE CALLED BY USER, CALLED IN ACDC_CLOCK.h) Sets the value for SysClock->LOAD
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void TIMER_SetSystemClockSpeed(SystemClockSpeed SCS_x);

/// @brief Initializes PWM output on the specified timer and channel with the given parameters. (Default duty cycle = 0)
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @param PWM_MODE_x Desired PWM mode
/// @param frequency  Desired frequency (in Hz) for PWM to run at
void TIMER_PWM_Init(TIMx_CHx TIMx_CHx_Pxx, PWM_MODE PWM_MODE_x, uint32_t frequency);

/// @brief Sets the duty cycle of the PWM signal on the specified timer and channel.
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @param dutyCycle Duty cycle value in timer ticks.
void TIMER_PWM_SetDuty(TIMx_CHx TIMx_CHx_Pxx, uint32_t dutyCycle);

/// @brief Retrieves the period of the PWM signal on the specified timer
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @return Period of the PWM signal in timer ticks.
uint32_t TIMER_PWM_GetPeriod(TIMx_CHx TIMx_CHx_Pxx);

/// @brief Grabs and returns the total number of milliseconds since the MCU turned on
/// @return Number of milliseconds since startup
uint64_t Millis();

/// @brief Grabs and returns the total number of microseconds since the MCU turned on
/// @return Number of microseconds since startup
uint64_t Micros();

/// @brief Pauses code execution for delayVal number of milliseconds
/// @param delayVal Number of milliseconds to delay for
void Delay_MS(uint64_t delayVal);

/// @brief Pauses code execution for delayVal number of microseconds
/// @param delayVal Number of microseconds to delay for
void Delay_US(uint64_t delayVal);

#endif