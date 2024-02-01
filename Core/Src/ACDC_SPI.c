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

void SPI_Initalize(SPI_TypeDef *SPI) {
    if(SPI == SPI1){
        // Enable clock for SPI
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        // Configure GPIO pins for SPI1
        // Set PB_3 (SCLK) as alternate function push-pull output
        GPIOB->CRL &= ~(GPIO_CRL_CNF3 | GPIO_CRL_MODE3);
        GPIOB->CRL |= (GPIO_CRL_CNF3_1 | GPIO_CRL_MODE3);

        // Set PB_4 (MISO) as input floating
        GPIOB->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_MODE4);

        // Set PB_5 (MOSI) as alternate function push-pull output
        GPIOB->CRL &= ~(GPIO_CRL_CNF5 | GPIO_CRL_MODE5);
        GPIOB->CRL |= (GPIO_CRL_CNF5_1 | GPIO_CRL_MODE5);

        // Configure SPI1
        SPI1->CR1 = 0; // Clear CR1 register
        SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_0 | SPI_CR1_BR_1; // Set master mode, software NSS, baud rate
        SPI1->CR1 |= SPI_CR1_MSTR; // Set as master

        // Enable SPI1
        SPI1->CR1 |= SPI_CR1_SPE;
    }
    else if(SPI == SPI2){
        // Enable clock for SPI2
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

        // Configure GPIO pins for SPI2
        // Set PB13 (SCK) as alternate function push-pull output
        GPIOB->CRH &= ~(GPIO_CRH_CNF13 | GPIO_CRH_MODE13);
        GPIOB->CRH |= GPIO_CRH_CNF13_1 | GPIO_CRH_MODE13_1;

        // Set PB14 (MISO) as input floating
        GPIOB->CRH &= ~(GPIO_CRH_CNF14 | GPIO_CRH_MODE14);

        // Set PB15 (MOSI) as alternate function push-pull output
        GPIOB->CRH &= ~(GPIO_CRH_CNF15 | GPIO_CRH_MODE15);
        GPIOB->CRH |= GPIO_CRH_CNF15_1 | GPIO_CRH_MODE15_1;

        // Configure SPI2
        SPI2->CR1 = 0; // Clear CR1 register
        SPI2->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_BR_0 | SPI_CR1_BR_1; // Set master mode, software NSS, baud rate
        SPI2->CR1 |= SPI_CR1_MSTR; // Set as master

        // Enable SPI2
        SPI2->CR1 |= SPI_CR1_SPE;
    }
    else{
        printf("Invalid SPI pointer.");
    }
}

void SPI_Transmit(uint16_t data, SPI_TypeDef *SPI) {
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
    else{
        printf("Invalid SPI pointer.");
    }
}

uint16_t SPI_Receive(SPI_TypeDef *SPI) {
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
        printf("Invalid SPI pointer.");
    }
}

uint16_t SPI_TransmitReceive(uint16_t data, SPI_TypeDef *SPI) {
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
        printf("Invalid SPI pointer.");
    }
}