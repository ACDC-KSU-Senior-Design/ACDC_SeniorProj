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

/// @brief Enables the USARTx peripheral clock (Needed for peripheral to function)
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
static void USART_InitClk(USART_TypeDef *USARTx);

/// @brief Initilizes the various pins for UART/USART {See RM-167, RM-168}
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param useUART true if you want UART, false if you want USART
static void USART_InitPin(USART_TypeDef *USARTx, bool useUART);

void USART_Init(USART_TypeDef *USARTx, SerialSpeed Serial_x, bool useUART){
    USART_InitClk(USARTx);
    USART_InitPin(USARTx, useUART);
   
    //SystemClockSpeed SCS_x = CLOCK_GetSystemClockSpeed();
    SystemClockSpeed SCS_x = SCS_64MHz; //CLOCK IS AT 64MHZ

    //0x391 if 115200 and 72Mhz
    //0x116 if 115200 and 12Mhz
    USARTx->BRR = 0x116; // Value on Page 799

    //39.0625   USARTDIV = fClk / (16 * Baud)
    //uint32_t usartDiv = 0x391;

    /*Enable USART, Tx, Rx*/
    SET_BIT(USARTx->CR1, USART_CR1_UE | USART_CR1_TE | USART_CR1_RE); //USART_CR1_TXE
    CLEAR_BIT(USARTx->CR1, USART_CR1_M | USART_CR1_PCE | USART_CR1_RWU);

    /*1 Stop Bit, */
    CLEAR_BIT(USARTx->CR2, USART_CR2_STOP);
}

void USART_SendChar(USART_TypeDef *USARTx, char chr){
    while(!READ_BIT(USARTx->SR, USART_SR_TXE)){} //Wait until buffer is ready to transmit again
    WRITE_REG(USARTx->DR, chr & USART_DR_DR_Msk);
}

void USART_SendString(USART_TypeDef *USARTx, char* str){
    int i = 0;
    while(str[i] != '\0'){
        USART_SendChar(USARTx, str[i++]);
    }
}

char USART_RecieveChar(USART_TypeDef *USARTx){
    return READ_REG(USARTx->DR & 0xFF); //Need to make sure it has data first
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

static void USART_InitPin(USART_TypeDef *USARTx, bool useUART){
    GPIO_TypeDef *GPIO_Port;
    uint16_t Tx_Pin, Rx_Pin, Ck_Pin, CTS_Pin, RTS_Pin;
    
    //Pin Definitions {See RM-181}
    if(USARTx == USART1){
        if(AFIO->MAPR & AFIO_MAPR_USART1_REMAP){    // USART1 does not have a CK, CTS and RTS pin (ONLY UART NO USART) {See RM-180}
            GPIO_Port = GPIOB;
            Tx_Pin = GPIO_PIN_6;
            Rx_Pin = GPIO_PIN_7;
        } else {
            GPIO_Port = GPIOA;
            Tx_Pin = GPIO_PIN_9;
            Rx_Pin = GPIO_PIN_10;
        }
    } else if(USARTx == USART3){
        if(AFIO->MAPR & AFIO_MAPR_USART3_REMAP_0)  // <- Partial Remap
            GPIO_Port = GPIOC;                     // Full remap only available on 100 pin & 144 pin packages {See RM-180}
        else 
            GPIO_Port = GPIOB;

        Tx_Pin = GPIO_PIN_10;   //Always Pin 10
        Rx_Pin = GPIO_PIN_11;   //Always Pin 11
        Ck_Pin = GPIO_PIN_12;   //Always Pin 12
        CTS_Pin = GPIO_PIN_13;  //Always Pin 13 & GPIOB
        RTS_Pin = GPIO_PIN_14;  //Always Pin 14 & GPIOB

        if(!useUART){
            GPIO_PinDirection(GPIO_Port, Ck_Pin , GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
            GPIO_PinDirection(GPIOB    , CTS_Pin, GPIO_MODE_AF_INPUT          , GPIO_CNF_INPUT_FLOATING);
            GPIO_PinDirection(GPIOB    , RTS_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
        }

    } else if (USARTx == USART2){   // Remap is only available on 100 & 144 pin packages {See RM-180}
        GPIO_Port = GPIOA;
        Tx_Pin = GPIO_PIN_2;
        Rx_Pin = GPIO_PIN_3;
        Ck_Pin = GPIO_PIN_4;
        CTS_Pin = GPIO_PIN_0;
        RTS_Pin = GPIO_PIN_1;

        if(!useUART){
            GPIO_PinDirection(GPIO_Port, Ck_Pin , GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
            GPIO_PinDirection(GPIO_Port, CTS_Pin, GPIO_MODE_AF_INPUT          , GPIO_CNF_INPUT_FLOATING);
            GPIO_PinDirection(GPIO_Port, RTS_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
        }
    } 

    GPIO_PinDirection(GPIO_Port, Tx_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    GPIO_PinDirection(GPIO_Port, Rx_Pin, GPIO_MODE_INPUT             , GPIO_CNF_INPUT_FLOATING);
}