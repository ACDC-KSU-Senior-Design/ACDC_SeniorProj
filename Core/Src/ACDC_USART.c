/**
 * @file ACDC_USART.c
 * @author Devin Marx
 * @brief Implentation of UART/USART Tx/Rx functions.
 * @version 0.2
 * @date 2024-01-21
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_USART.h"
#include "ACDC_GPIO.h"
#include "ACDC_string.h"

static void USART_InitClk(USART_TypeDef *USARTx);
static void USART_InitPin(USART_TypeDef *USARTx);

void USART_Init(USART_TypeDef *USARTx, SerialSpeed Serial_x, SystemClockSpeed SCS_x){
    USART_InitClk(USARTx);
    USART_InitPin(USARTx);
    //0x391 if 115200 and 72Mhz
    //0x116 if 115200 and 12Mhz
    USARTx->BRR = 0x116; // Value on Page 799

    //39.0625   USARTDIV = fClk / (16 * Baud)
    //uint32_t usartDiv = 0x391;

    /*Enable USART, Tx, Rx*/
    USARTx->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;    //USART_CR1_TXE
    USARTx->CR1 &= ~(USART_CR1_M | USART_CR1_PCE | USART_CR1_RWU);

    /*1 Stop Bit, */
    USARTx->CR2 &= ~(USART_CR2_STOP);
}

void USART_SendChar(USART_TypeDef *USARTx, char chr){
    while(!(USARTx->SR & USART_SR_TXE)){} // Wait until 
    USARTx->DR = chr & USART_DR_DR_Msk;
}

void USART_SendString(USART_TypeDef *USARTx, char* str){
    int i = 0;
    while(str[i] != '\0'){
        USART_SendChar(USARTx, str[i++]);
    }
}

char USART_RecieveChar(USART_TypeDef *USARTx){
    return USARTx->DR;  //Need to make sure it has data first
}

void USART_RecieveString(USART_TypeDef *USARTx, char* buffer){}

static void USART_InitClk(USART_TypeDef *USARTx){
    if(USARTx == USART1)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);    //Enable USART1 Clock
    else if(USARTx == USART2)
        SET_BIT(RCC->APB2ENR, RCC_APB1ENR_USART2EN);    //Enable USART2 Clock
    else if(USARTx == USART3)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);    //Enable USART3 Clock
}

/// @brief Check Page 166-167, USART_Tx = Alternate Function Push-Pull, USART_Rx = Input Floating / Input Pullup
/// @param USARTx 
static void USART_InitPin(USART_TypeDef *USARTx){
    GPIO_TypeDef *GPIO_Port;
    uint16_t Tx_Pin, Rx_Pin;
    
    //Page 181
    if(USARTx == USART1){
        if(AFIO->MAPR & AFIO_MAPR_USART1_REMAP){
            GPIO_Port = GPIOB;
            Tx_Pin = GPIO_PIN_6;
            Rx_Pin = GPIO_PIN_7;
        } else {
            GPIO_Port = GPIOA;
            Tx_Pin = GPIO_PIN_9;
            Rx_Pin = GPIO_PIN_10;
        }
    } else if(USARTx == USART3){
        if(AFIO->MAPR & AFIO_MAPR_USART3_REMAP_0){
            GPIO_Port = GPIOC;
            Tx_Pin = GPIO_PIN_10;
            Rx_Pin = GPIO_PIN_11;
        } else {
            GPIO_Port = GPIOB;
            Tx_Pin = GPIO_PIN_10;
            Rx_Pin = GPIO_PIN_11;
        }
    } else if (USARTx == USART2){
        GPIO_Port = GPIOA;
        Tx_Pin = GPIO_PIN_2;
        Rx_Pin = GPIO_PIN_3;
    } 

    GPIO_PinDirection(GPIO_Port, Tx_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    GPIO_PinDirection(GPIO_Port, Rx_Pin, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
}