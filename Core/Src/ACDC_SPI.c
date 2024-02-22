/**
 * @file ACDC_SPI.h
 * @author Liam Bramley, Devin Marx
 * @brief Implementation of SPI driver
 * 
 * This file includes functions to initalize the SPI, transfer and recieve data.
 * 
 * @version 0.1
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_SPI.h"

void SPI_Init(SPI_TypeDef *SPIx) {
    if(SPIx == SPI1){
        // Enable clock for SPI
        RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

        // Configure GPIO pins for SPI1
        // Set PA5 (SCK) as alternate function push-pull output
        GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Set PA6 (MISO) as input floating
        GPIO_PinDirection(GPIOA, GPIO_PIN_6, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);

        // Set PA7 (MOSI) as alternate function push-pull output
        GPIO_PinDirection(GPIOA, GPIO_PIN_7, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    }
    else if(SPIx == SPI2){
        // Enable clock for SPI2
        RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;

        // Configure GPIO pins for SPI2
        // Set PB13 (SCK) as alternate function push-pull output
        GPIO_PinDirection(GPIOB, GPIO_PIN_13, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

        // Set PB14 (MISO) as input floating
        GPIO_PinDirection(GPIOB, GPIO_PIN_14, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);

        // Set PB15 (MOSI) as alternate function push-pull output
        GPIO_PinDirection(GPIOB, GPIO_PIN_15, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    }

    // Configure SPIx
    SPIx->CR1 &= ~(SPI_CR1_SPE);            // Disable SPIx
    SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI; // Set master mode, software NSS
    SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_2);
    SPI_SetBitMode(SPIx, SPI_MODE_16Bit);
    SPI_SetLsbFirst(SPIx, false);
    SPIx->CR1 |= SPI_CR1_MSTR; // Set as master

    // Enable SPI1
    SPIx->CR1 |= SPI_CR1_SPE;
}

void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data) {
    // Wait until transmit buffer is empty
    while (!(SPIx->SR & SPI_SR_TXE));

    // Send data
    SPIx->DR = data;
}

uint16_t SPI_Receive(const SPI_TypeDef *SPIx) {
    // Wait until receive buffer is full
    while (!(SPIx->SR & SPI_SR_RXNE));

    // Return received data
    return SPIx->DR;
}

uint16_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint16_t data) {
    SPI_Transmit(SPIx, data);
    return SPI_Receive(SPIx);
}

void SPI_SetBaudDivider(SPI_TypeDef *SPIx, SPI_BaudDivider SPI_BAUD_DIV_x){
    while(READ_BIT(SPIx->SR, SPI_SR_BSY)){}                 // While SPIx is busy in communication or Tx buffer is not empty
    CLEAR_BIT(SPIx->CR1, SPI_CR1_BR_Msk);                   // Clear the SPIx Baud Divisor bits
    SET_BIT(SPIx->CR1, SPI_BAUD_DIV_x << SPI_CR1_BR_Pos);   // Set the SPIx Baud Divisor bits
}

void SPI_SetBitMode(SPI_TypeDef *SPIx, SPI_BitMode SPI_MODE_x){
    //Note: This bit should be written only when SPI is disabled (SPE = ‘0’) for correct operation {See RM-742}
    bool wasInitialized = READ_BIT(SPIx->CR1, SPI_CR1_SPE) ? true : false;
    if(wasInitialized)                      // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    if(SPI_MODE_x == SPI_MODE_8Bit)         // If the user wants 8-bit mode
        CLEAR_BIT(SPIx->CR1, SPI_CR1_DFF);  // Set to 8-bit mode
    else if(SPI_MODE_x == SPI_MODE_16Bit)   // If the user wants 16-bit mode
        SET_BIT(SPIx->CR1, SPI_CR1_DFF);    // Set to 16-bit mode

    if(wasInitialized)                      // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

void SPI_SetLsbFirst(SPI_TypeDef *SPIx, bool LsbFirst){
    //Note: This bit should not be changed when communication is ongoing {See RM-743}
    while(READ_BIT(SPIx->SR, SPI_SR_BSY)){}     // While SPIx is busy in communication or Tx buffer is not empty
    if(LsbFirst)
        SET_BIT(SPIx->CR1  , SPI_CR1_LSBFIRST); // Set LSB first
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_LSBFIRST); // Set MSB first
}