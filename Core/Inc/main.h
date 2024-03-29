/**
 * @file main.h
 * @author STMicroelectronics
 * @brief  Header for main.c file. This file contains the common defines of the application.
 * @copyright 
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 * @attention
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx.h"
#include "stm32f1xx_hal.h"
#include "ACDC_stdbool.h"
#include "ACDC_stdint.h"
#include "ACDC_string.h"
#include "ACDC_GPIO.h"
#include "ACDC_CLOCK.h"
#include "ACDC_INTERRUPT.h"
#include "ACDC_TIMER.h"
#include "ACDC_USART.h"
#include "ACDC_SPI.h"
#include "ACDC_LTC1298_ADC.h"
#include "ACDC_LTC1451_DAC.h"

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
