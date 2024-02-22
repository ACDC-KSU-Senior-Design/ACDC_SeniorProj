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

#pragma region PRIVATE_FUNCTION_PROTOTYPES
/// @brief Enables the SPIx peripheral clock (Needed for peripheral to function)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
static void SPI_InitClk(const SPI_TypeDef *SPIx);

/// @brief Initilizes the various pins for SPI {See RM-181, DS-30}
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster true if SPIx should act as the master, false if it should act as a slave
static void SPI_InitPin(const SPI_TypeDef *SPIx, bool isMaster);
#pragma endregion

void SPI_Init(SPI_TypeDef *SPIx) {
    SPI_InitClk(SPIx);
    SPI_InitPin(SPIx, true);

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

#pragma region PRIVATE_FUNCTIONS
static void SPI_InitClk(const SPI_TypeDef *SPIx){
    if(SPIx == SPI1)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_SPI1EN);
    else if(SPIx == SPI2)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_SPI2EN);
}

static void SPI_InitPin(const SPI_TypeDef *SPIx, bool isMaster){
    GPIO_TypeDef *GPIO_PORT = NULL;
    uint16_t SPI_SCK = 0, SPI_MISO = 0, SPI_MOSI = 0;

    if(SPIx == SPI1){   //SPI1 Remapping {See RM-181}
        if(READ_BIT(AFIO->MAPR, AFIO_MAPR_SPI1_REMAP)){ // IF SPI1 is remapped
            GPIO_PORT = GPIOB;
            SPI_SCK  = GPIO_PIN_3;
            SPI_MISO = GPIO_PIN_4;
            SPI_MOSI = GPIO_PIN_5;
        } else {    // SPI1 is not remapped
            GPIO_PORT = GPIOA;
            SPI_SCK  = GPIO_PIN_5;
            SPI_MISO = GPIO_PIN_6;
            SPI_MOSI = GPIO_PIN_7;
        }
    } else if(SPIx == SPI2){
        GPIO_PORT = GPIOB;      // Pin Mappings {See DS-30}
        SPI_SCK  = GPIO_PIN_13;
        SPI_MISO = GPIO_PIN_14;
        SPI_MOSI = GPIO_PIN_15;
    }

    if(isMaster){   // Pin Configuration {See RM-167}
        GPIO_PinDirection(GPIO_PORT, SPI_SCK , GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
        GPIO_PinDirection(GPIO_PORT, SPI_MISO, GPIO_MODE_INPUT             , GPIO_CNF_INPUT_FLOATING     );
        GPIO_PinDirection(GPIO_PORT, SPI_MOSI, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
    } else {        // isSlave
        GPIO_PinDirection(GPIO_PORT, SPI_SCK , GPIO_MODE_INPUT             , GPIO_CNF_INPUT_FLOATING     );
        GPIO_PinDirection(GPIO_PORT, SPI_MISO, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL); // Point to Point (Use Open drain if multi-slave)
        GPIO_PinDirection(GPIO_PORT, SPI_MOSI, GPIO_MODE_INPUT             , GPIO_CNF_INPUT_FLOATING     );
    }
}
#pragma endregion