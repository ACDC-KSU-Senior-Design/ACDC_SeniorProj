/**
 * @file ACDC_CLOCK.h
 * @author Devin Marx
 * @brief Header file containing functions for configuring the system clock and clock-related peripherals.
 * 
 * This file defines enums and functions for configuring the system clock speed, enabling the MCO output,
 * setting ADC and APB prescalers, and retrieving the current system clock speed.
 * 
 * @version 0.1
 * @date 2023-11-3
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_CLOCK_H
#define __ACDC_CLOCK_H

#include "stm32f1xx.h"
#include "ACDC_stdbool.h"

typedef enum{   /* System Clock Speed */
    SCS_1MHz  = 1000000,  /**< System Clock Speed 1MHz  */
    SCS_2MHz  = 2000000,  /**< System Clock Speed 2MHz  */
    SCS_3MHz  = 3000000,  /**< System Clock Speed 3MHz  */
    SCS_4MHz  = 4000000,  /**< System Clock Speed 4MHz  */
    SCS_5MHz  = 5000000,  /**< System Clock Speed 5MHz  */
    SCS_6MHz  = 6000000,  /**< System Clock Speed 6MHz  */
    SCS_7MHz  = 7000000,  /**< System Clock Speed 7MHz  */
    SCS_8MHz  = 8000000,  /**< System Clock Speed 8MHz  */
    SCS_9MHz  = 9000000,  /**< System Clock Speed 9MHz  */
    SCS_10MHz = 10000000, /**< System Clock Speed 10MHz */
    SCS_11MHz = 11000000, /**< System Clock Speed 11MHz */
    SCS_12MHz = 12000000, /**< System Clock Speed 12MHz */
    SCS_13MHz = 13000000, /**< System Clock Speed 13MHz */
    SCS_14MHz = 14000000, /**< System Clock Speed 14MHz */
    SCS_15MHz = 15000000, /**< System Clock Speed 15MHz */
    SCS_16MHz = 16000000, /**< System Clock Speed 16MHz */
    SCS_18MHz = 18000000, /**< System Clock Speed 18MHz */
    SCS_20MHz = 20000000, /**< System Clock Speed 20MHz */
    SCS_22MHz = 22000000, /**< System Clock Speed 22MHz */
    SCS_24MHz = 24000000, /**< System Clock Speed 24MHz */
    SCS_26MHz = 26000000, /**< System Clock Speed 26MHz */
    SCS_28MHz = 28000000, /**< System Clock Speed 28MHz */
    SCS_30MHz = 30000000, /**< System Clock Speed 30MHz */
    SCS_32MHz = 32000000, /**< System Clock Speed 32MHz */
    SCS_36MHz = 36000000, /**< System Clock Speed 36MHz */
    SCS_40MHz = 40000000, /**< System Clock Speed 40MHz */
    SCS_44MHz = 44000000, /**< System Clock Speed 44MHz */
    SCS_48MHz = 48000000, /**< System Clock Speed 48MHz */
    SCS_52MHz = 52000000, /**< System Clock Speed 52MHz */
    SCS_56MHz = 56000000, /**< System Clock Speed 56MHz */
    SCS_60MHz = 60000000, /**< System Clock Speed 60MHz */
    SCS_64MHz = 64000000, /**< System Clock Speed 64MHz */
    SCS_72MHz = 72000000  /**< System Clock Speed 72MHz */
}SystemClockSpeed;

typedef enum{   /* MCO Clock Output Control */
    MCO_NO_CLK   = RCC_CFGR_MCO_NOCLOCK,    /**< No Clock Output        */
    MCO_SYSCLK   = RCC_CFGR_MCO_SYSCLK,     /**< System Clock output    */
    MCO_HSICLK   = RCC_CFGR_MCO_HSI,        /**< HSI Clock output       */
    MCO_HSECLK   = RCC_CFGR_MCO_HSE,        /**< HSE Clock output       */
    MCO_PLLCLK   = RCC_CFGR_MCO_PLLCLK_DIV2 /**< PLL Clock Divided by 2 */
}MicroClockOutput;

typedef enum{   /* ADC Clock Prescaler */
    ADC_DIV_2   = RCC_CFGR_ADCPRE_DIV2, /**< CLK divided by 2 */
    ADC_DIV_4   = RCC_CFGR_ADCPRE_DIV4, /**< CLK divided by 4 */
    ADC_DIV_6   = RCC_CFGR_ADCPRE_DIV6, /**< CLK divided by 6 */
    ADC_DIV_8   = RCC_CFGR_ADCPRE_DIV8, /**< CLK divided by 8 */
}ADC_Prescaler;

typedef enum{   /* APBx Clock Prescaler */
    APB_DIV_1   = RCC_CFGR_PPRE1_DIV1,  /**< CLK not divided   */
    APB_DIV_2   = RCC_CFGR_PPRE1_DIV2,  /**< CLK divided by 2  */
    APB_DIV_4   = RCC_CFGR_PPRE1_DIV4,  /**< CLK divided by 4  */
    APB_DIV_8   = RCC_CFGR_PPRE1_DIV8,  /**< CLK divided by 8  */ 
    APB_DIV_16  = RCC_CFGR_PPRE1_DIV16, /**< CLK divided by 16 */
}APB_Prescaler;    

/// @brief Sets the SYSCLK speed, and sets the perepherals to their fastest speed available
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void CLOCK_SetSystemClockSpeed(SystemClockSpeed SCS_x);

/// @brief Retrieves the current SYSCLK.
/// @return System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
SystemClockSpeed CLOCK_GetSystemClockSpeed(void);

/// @brief Retrieves the clock speed of the APB1 Clock
/// @return System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
SystemClockSpeed CLOCK_GetAPB1ClockSpeed(void);

/// @brief Retrieves the clock speed of the APB2 Clock
/// @return System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
SystemClockSpeed CLOCK_GetAPB2ClockSpeed(void);

/// @brief Enables the MCO output on Pin PA8 using MCO_x as the clock source
/// @param MCO_x Clock output source (Ex. MCO_NO_CLK, MCO_SYSCLK, ...)
void CLOCK_SetMcoOutput(MicroClockOutput MCO_x);

/// @brief Sets the ADC's Prescaler to either 2, 4, 6, 8
/// @param PRE_DIV_x ADC Prescaler (Ex. ADC_DIV_2, ADC_DIV_4, ...)
void CLOCK_SetADCPrescaler(ADC_Prescaler ADC_DIV_x);

/// @brief Sets the Advanced Peripheral Bus 1 Prescaler to either 1, 2, 4, 8, 16
/// @param APB_DIV_x APB1 Prescaler Divider (Ex. APB_DIV_1, APB_DIV_2, ...)
void CLOCK_SetAPB1Prescaler(APB_Prescaler APB_DIV_x);

/// @brief Sets the Advanced Peripheral Bus 2 Prescaler to either 1, 2, 4, 8, 16
/// @param APB_DIV_x APB2 Prescaler Divider (Ex. APB_DIV_1, APB_DIV_2, ...)
void CLOCK_SetAPB2Prescaler(APB_Prescaler APB_DIV_x);

#endif