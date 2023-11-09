#ifndef __ACDC_USART_H
#define __ACDC_USART_H

#include "stm32f103xb.h"

typedef enum{
    Serial_1200,
    Serial_2400,
    Serial_4800,
    Serial_9600,
    Serial_19200,
    Serial_38400,
    Serial_57600,
    Serial_115200
}SerialSpeed;

USART_TypeDef* USART_Init(USART_TypeDef *USARTx, SerialSpeed Serial_x);

void USART_SendChar(USART_TypeDef *USARTx, char chr);

void USART_SendString(USART_TypeDef *USARTx, char* str);

char USART_RecieveChar(USART_TypeDef *USARTx);

void USART_RecieveString(USART_TypeDef *USARTx, char* buffer);

#endif


// /** 
//   * @brief Universal Synchronous Asynchronous Receiver Transmitter
//   */
 
// typedef struct
// {
//   __IO uint32_t SR;         /* USART Status register,                   Address offset: 0x00 */
//   __IO uint32_t DR;         /* USART Data register,                     Address offset: 0x04 */
//   __IO uint32_t BRR;        /* USART Baud rate register,                Address offset: 0x08 */
//   __IO uint32_t CR1;        /* USART Control register 1,                Address offset: 0x0C */
//   __IO uint32_t CR2;        /* USART Control register 2,                Address offset: 0x10 */
//   __IO uint32_t CR3;        /* USART Control register 3,                Address offset: 0x14 */
//   __IO uint32_t GTPR;       /* USART Guard time and prescaler register, Address offset: 0x18 */
// } USART_TypeDef;