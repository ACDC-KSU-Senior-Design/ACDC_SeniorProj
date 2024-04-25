/**
 * @file main.c
 * @author STMicroelectronics
 * @brief  Main program body
 * @copyright 
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 * @attention
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/// @brief Initializes the system clock, internal peripherals, and external hardware
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void ACDC_Init(SystemClockSpeed SCS_x);

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  ACDC_Init(SCS_72MHz);

  TIMER_PWM_Init(TIM3_CH2_PA7, PWM_MODE_1, 100000);
  uint32_t timPeriod = TIMER_PWM_GetPeriod(TIM3_CH2_PA7);
  int8_t incrementValue = 10;

  while (1)
  {
    int64_t currVal = TIMER_PWM_GetDuty(TIM3_CH2_PA7);                          // Get the current duty value
    if(currVal + incrementValue <= 0 || currVal + incrementValue >= timPeriod)  // If the new value is less than 0 or greator than the period
      incrementValue *= -1;                                                     // Change the incrementation direction
    
    TIMER_PWM_SetDuty(TIM3_CH2_PA7, currVal + incrementValue);                  // Set the new value of the LED
    Delay_MS(10);
  }
}

void ACDC_Init(SystemClockSpeed SCS_x){
  CLOCK_SetSystemClockSpeed(SCS_x);
  //APB1 & APB2 Prescalers are set the highest speed in CLOCK_SetSystemClockSpeed

  USART_Init(USART2, Serial_115200, true);  // Initilize USART2 with a baud of 115200

  TIMER_PWM_Init(TIM1_CH4_PA11, PWM_MODE_1, 1000000);                         // Needed to drive the ARINC429 Clock
  TIMER_PWM_SetDuty(TIM1_CH4_PA11, TIMER_PWM_GetPeriod(TIM1_CH4_PA11) / 2);   // Set the Duty cycle to 50%

  CLOCK_SetMcoOutput(MCO_SYSCLK);     //Sets PA8 as the output of SysClock
  GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_PUSH_PULL);
  GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);  //External Pullup/down resistor
  GPIO_INT_SetToInterrupt(GPIOC, GPIO_PIN_13, TT_RISING_EDGE); 
}


/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
