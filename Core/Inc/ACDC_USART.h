/**
 * @file ACDC_USART.h
 * @author Devin Marx
 * @brief Header file containing various functions for sending, receiving, and initilizing the UART/USART prehiperal
 * @version 0.1
 * @date 2023-11-8
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_USART_H
#define __ACDC_USART_H

#include "stm32f1xx.h"
#include "ACDC_CLOCK.h"

typedef enum{ // UART/USART Serial Speed
    Serial_1200   = 1200,   /**< Baud rate: 1200 bps   */
    Serial_2400   = 2400,   /**< Baud rate: 2400 bps   */
    Serial_4800   = 4800,   /**< Baud rate: 4800 bps   */
    Serial_9600   = 9600,   /**< Baud rate: 9600 bps   */
    Serial_19200  = 19200,  /**< Baud rate: 19200 bps  */
    Serial_38400  = 38400,  /**< Baud rate: 38400 bps  */
    Serial_57600  = 57600,  /**< Baud rate: 57600 bps  */
    Serial_115200 = 115200, /**< Baud rate: 115200 bps */
    Serial_230400 = 230400  /**< Baud rate: 230400 bps */
}SerialSpeed;

/// @brief Initilizes the USARTx peripheral at the serial speed Serial_x, and the default 
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param Serial_x Tx/Rx speed of the USART peripheral (Ex. Serial_115200, Serial_9600, ...)
/// @param SystemClockSpeed true if you want UART, false if you want USART
void USART_Init(USART_TypeDef *USARTx, SerialSpeed Serial_x, bool useUART);

/// @brief Changes the Serial speed of the USART 
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param Serial_x Tx/Rx speed of the USART peripheral (Ex. Serial_115200, Serial_9600, ...)
void USART_ChangeSerialSpeed(USART_TypeDef *USARTx, SerialSpeed Serial_x);

/// @brief Sends a single character over UART/USART
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param chr Signular character to send over UART/USART
void USART_SendChar(USART_TypeDef *USARTx, char chr);

/// @brief Sends a null-terminaled string over UART/USART
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param str String to send over UART/USART
void USART_SendString(USART_TypeDef *USARTx, char* str);

/// @brief Recieves a single character from  UART/USART
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @return Character recieved over UART/USART
char USART_RecieveChar(USART_TypeDef *USARTx);

/// @brief Recieves a string from UART/USART and stops when a '\0' is recieved or the buffer is full.
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param buffer Buffer to store the recieved string
/// @param bufferLen Maximum length of the buffer
void USART_RecieveString(USART_TypeDef *USARTx, char* buffer, uint16_t bufferLen);

/// @brief Checks if there is data available in the USART recieve buffer.
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @return True if there is data available to recieve, false otherwise.
bool USART_HasDataToRecieve(USART_TypeDef *USARTx);

#endif