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
UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void ACDC_Init(SystemClockSpeed SCS_x);

char recieveBuffer[50];

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  ACDC_Init(SCS_72MHz);

  SPI_Init(SPI1, true); // Initialize SPI1 to master
  SPI_SetBaudDivider(SPI1, SPI_BAUD_DIV_256);
  GPIO_PinDirection(GPIOB, GPIO_PIN_6, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_PUSH_PULL);  // Set the CS pin to an PP output
 
  uint32_t time = Millis();

  while (1)
  {
    if(Millis() - time >= 1000){      // Every 1 Second
      GPIO_Clear(GPIOB, GPIO_PIN_6);  // Set CS low
      SPI_Transmit(SPI1, 0b11011010); // Send Random data
      while(SPI1->SR & SPI_SR_BSY){}  // Wait until data has been transmitted
      GPIO_Set(GPIOB, GPIO_PIN_6);    // Finish and set CS high
      time = Millis();                // Update the time variable
    }

    if(SPI_HasDataToRecieve(SPI1)){
      // Need to set CS low first
      uint8_t recievedData = SPI_Receive(SPI1);
      SPI_Transmit(SPI1, 0);
      SPI_Transmit(SPI1, recievedData); // Should send the recieved data with 8 leading and following 0's
      SPI_Transmit(SPI1, 0);
    }
  }
}

static void ACDC_Init(SystemClockSpeed SCS_x){
  CLOCK_SetSystemClockSpeed(SCS_x);   //72Mhz doesnt quite work Rn
  CLOCK_SetAPB1Prescaler(APB_DIV_2);  //Max Speed is 36Mhz
  CLOCK_SetAPB2Prescaler(APB_DIV_1);  //Max Speed is 72Mhz

  //USART_Init(USART2, Serial_115200, true);  // Initilize USART2 with a baud of 115200

  CLOCK_SetMcoOutput(MCO_SYSCLK);     //Sets PA8 as the output of SysClock
  GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_PUSH_PULL);
  GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);  //External Pullup/down resistor
  GPIO_INT_SetToInterrupt(GPIOC, GPIO_PIN_13, TT_RISING_EDGE);  
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
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
