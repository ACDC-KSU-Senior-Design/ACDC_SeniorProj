/**
 * @file ACDC_SPI.h
 * @author Liam Bramley, Devin Marx
 * @brief Implementation of SPI driver
 * 
 * This file includes functions to initalize the SPI, transfer and recieve data.
 * 
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_SPI.h"
#include "ACDC_GPIO.h"

static uint8_t SPI_Initialized = 0; // Private variable that detects if SPI1 & SPI2 have been initalized

#pragma region PRIVATE_FUNCTION_PROTYPES
/// @brief Enables the SPIx peripheral clock (Needed for peripheral to function)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
static void SPI_InitClk(const SPI_TypeDef *SPIx);

/// @brief Initilizes the various pins for SPI {See RM-181, DS-30}
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster true if SPIx should act as the master, false if it should act as a slave
static void SPI_InitPin(const SPI_TypeDef *SPIx, bool isMaster);

/// @brief Checks if the current SPIx peripheral has been initialized.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
static bool SPI_IsInitialized(const SPI_TypeDef *SPIx);

/// @brief Sets the initialization status of the current SPIx peripheral.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
static void SPI_SetInitialized(const SPI_TypeDef *SPIx);
#pragma endregion

void SPI_Init(SPI_TypeDef *SPIx, bool isMaster) {
    //Configure SPI in Master Mode {See RM-707}
    //Configure SPI in Slave Mode {See RM-706}

    SPI_InitClk(SPIx);
    SPI_InitPin(SPIx, isMaster);

    CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE); // Disable SPIx

    // Configure SPIx
    SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_2);           // Baud Div = Fck / 2
    SPI_SetBitMode(SPIx, SPI_MODE_8Bit);                // 8-bit mode
    SPI_SetLsbFirst(SPIx, false);                       // MsbFirst
    SPI_SetClockPhaseAndPolarity(SPIx, true, false);    // 2nd Clk transmission is when data is captured, and Clk idles at 0
    SPI_SetToMaster(SPIx, isMaster);                    // Set to Master or Slave
    SPIx->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI;             // Set software CS, software NSS

    SET_BIT(SPIx->CR1, SPI_CR1_SPE); // Enable SPIx
    SPI_SetInitialized(SPIx);
}

void SPI_SetBaudDivider(SPI_TypeDef *SPIx, SPI_BaudDivider SPI_BAUD_DIV_x){
    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    CLEAR_BIT(SPIx->CR1, SPI_CR1_BR_Msk);                   // Clear the SPIx Baud Divisor bits
    SET_BIT(SPIx->CR1, SPI_BAUD_DIV_x << SPI_CR1_BR_Pos);   // Set the SPIx Baud Divisor bits

    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

void SPI_SetBitMode(SPI_TypeDef *SPIx, SPI_BitMode SPI_MODE_x){
    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    if(SPI_MODE_x == SPI_MODE_8Bit)         // If the user wants 8-bit mode
        CLEAR_BIT(SPIx->CR1, SPI_CR1_DFF);  // Set to 8-bit mode
    else if(SPI_MODE_x == SPI_MODE_16Bit)   // If the user wants 16-bit mode
        SET_BIT(SPIx->CR1, SPI_CR1_DFF);    // Set to 16-bit mode

    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

void SPI_SetLsbFirst(SPI_TypeDef *SPIx, bool LsbFirst){
    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    if(LsbFirst)
        SET_BIT(SPIx->CR1  , SPI_CR1_LSBFIRST);
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_LSBFIRST);

    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

void SPI_SetClockPhaseAndPolarity(SPI_TypeDef *SPIx, bool ClkPhase, bool ClkPolarity){
    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    if(ClkPhase)                                
        SET_BIT(SPIx->CR1  , SPI_CR1_CPHA);     // The second clock transition is the first data capture edge
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_CPHA);     // The first clock transition is the first data capture edge

    if(ClkPolarity)
        SET_BIT(SPIx->CR1  , SPI_CR1_CPOL);     // Set Clock to 1 or High when idle
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_CPOL);     // Set Clock to 0 or Low when idle

    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

void SPI_SetToMaster(SPI_TypeDef *SPIx, bool isMaster){
    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized
        CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);  // Disable SPIx
    
    if(isMaster)
        SET_BIT(SPIx->CR1, SPI_CR1_MSTR); // Set as master, (Default: Slave)
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_MSTR);

    if(SPI_IsInitialized(SPIx))             // If the peripheral was already initialized 
        SET_BIT(SPIx->CR1, SPI_CR1_SPE);    // Enable SPIx
}

bool SPI_HasDataToRecieve(SPI_TypeDef *SPIx){
    return READ_BIT(SPIx->SR, SPI_SR_RXNE) ? true : false; // Checks if the SPIx's recieve buffer is not empty
}

void SPI_EnableSoftwareCS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    UNUSED(SPIx);
    UNUSED(GPIOx);
    UNUSED(GPIO_PIN);
}
void SPI_EnableHardwareCS(SPI_TypeDef *SPIx){
    UNUSED(SPIx);
}

void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data) {
    // Wait until transmit buffer is empty
    while (!(SPIx->SR & SPI_SR_TXE));

    // Send data
    SPIx->DR = data;
}

uint16_t SPI_Receive(SPI_TypeDef *SPIx) {
    // Wait until receive buffer is full
    while (!(SPIx->SR & SPI_SR_RXNE));

    // Return received data
    return SPIx->DR;
}

uint16_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint16_t data) {
    // Wait until transmit buffer is empty
    while (!(SPIx->SR & SPI_SR_TXE));

    // Send data
    SPIx->DR = data;

    // Wait until receive buffer is full
    while (!(SPIx->SR & SPI_SR_RXNE));

    // Return received data
    return SPIx->DR;
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
    uint16_t SPI_SCK = 0, SPI_MISO = 0, SPI_MOSI = 0; //SPI_NSS = 0

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

static bool SPI_IsInitialized(const SPI_TypeDef *SPIx){
    if(SPIx == SPI1)
        return READ_BIT(SPI_Initialized, 0b01);
    else if(SPIx == SPI2)
        return READ_BIT(SPI_Initialized, 0b10);
}

static void SPI_SetInitialized(const SPI_TypeDef *SPIx){
    if(SPIx == SPI1)
        SET_BIT(SPI_Initialized, 0b01);
    else if(SPIx == SPI2)
        SET_BIT(SPI_Initialized, 0b10);
}
#pragma endregion