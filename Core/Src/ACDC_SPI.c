/**
 * @file ACDC_SPI.h
 * @author Liam Bramley
 * @brief Implementation of SPI driver
 * 
 * This file includes functions to initalize the SPI, transfer and recieve data.
 * 
 * @version 0.1
 * @date 2024-01-30
 * @copyright Copyright (c) 2023-2024
 * 
 */

#include "ACDC_SPI.h"


/*
{
  __IO uint32_t CR1;      /* SPI control register 1         */
  __IO uint32_t CR2;      /* SPI control register 2         */
  __IO uint32_t SR;       /* SPI status register            */
  __IO uint32_t DR;       /* SPI data register              */
  __IO uint32_t CRCPR;    /* SPI CRC polynomial register    */
  __IO uint32_t RXCRCR;   /* SPI RX CRC register            */
  __IO uint32_t TXCRCR;   /* SPI TX CRC register            */
  __IO uint32_t I2SCFGR;  /* SPI_I2S configuration register 
 SPI_TypeDef;
*/

void SPI_Initalize(const SPI_TypeDef *SPI) {
    if(SPI == SPI1){
        RCC-> APB2ENR |= RCC_APB2ENR_IOPBEN;

        // Enable clock for SPI
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        // Configure GPIO pins for SPI1
        // Set PA5 (SCK) as alternate function push-pull output
        GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Set PA6 (MISO) as input floating
        GPIO_PinDirection(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);

        // Set PA7 (MOSI) as alternate function push-pull output
        GPIO_PinDirection(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Configure SPI1
        SPI1->CR1 = 0; // Clear CR1 register
        SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_0 | SPI_CR1_BR_1; // Set master mode, software NSS, baud rate
        SPI1->CR1 |= SPI_CR1_MSTR; // Set as master

        // Enable SPI1
        SPI1->CR1 |= SPI_CR1_SPE;
    }
    else if(SPI == SPI2){
        RCC-> APB1ENR |= RCC_APB1ENR_I2C2EN;

        // Enable clock for SPI2
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

        // Configure GPIO pins for SPI2
        // Set PB13 (SCK) as alternate function push-pull output
        GPIO_PinDirection(GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Set PB14 (MISO) as input floating
        GPIO_PinDirection(GPIOB, GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);

        // Set PB15 (MOSI) as alternate function push-pull output
        GPIO_PinDirection(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Configure SPI2
        SPI2->CR1 = 0; // Clear CR1 register
        SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_0 | SPI_CR1_BR_1; // Set master mode, software NSS, baud rate
        SPI2->CR1 |= SPI_CR1_MSTR; // Set as master

        // Enable SPI2
        SPI2->CR1 |= SPI_CR1_SPE;
    }
}

void SPI_Transmit(uint16_t data, const SPI_TypeDef *SPI) {
    if(SPI == SPI1){
        // Wait until transmit buffer is empty
        while (!(SPI1->SR & SPI_SR_TXE));

        // Send data
        SPI1->DR = data;
    }
    else if(SPI == SPI2){
        // Wait until transmit buffer is empty
        while (!(SPI2->SR & SPI_SR_TXE));

        // Send data
        SPI2->DR = data;
    }
}

uint16_t SPI_Receive(const SPI_TypeDef *SPI) {
    if(SPI == SPI1){
        // Wait until receive buffer is full
        while (!(SPI1->SR & SPI_SR_RXNE));

        // Return received data
        return SPI1->DR;
    }
    else if(SPI == SPI2){
        // Wait until receive buffer is full
        while (!(SPI2->SR & SPI_SR_RXNE));

        // Return received data
        return SPI2->DR;
    }
    else{
        return 0;
    }
}

uint16_t SPI_TransmitReceive(uint16_t data, const SPI_TypeDef *SPI) {
    if(SPI == SPI1){
        // Wait until transmit buffer is empty
        while (!(SPI1->SR & SPI_SR_TXE));

        // Send data
        SPI1->DR = data;

        // Wait until receive buffer is full
        while (!(SPI1->SR & SPI_SR_RXNE));

        // Return received data
        return SPI1->DR;
    }
    else if(SPI == SPI2){
        // Wait until transmit buffer is empty
        while (!(SPI2->SR & SPI_SR_TXE));

        // Send data
        SPI2->DR = data;

        // Wait until receive buffer is full
        while (!(SPI2->SR & SPI_SR_RXNE));

        // Return received data
        return SPI2->DR;
    }
    else{
        return 0;
    }
}