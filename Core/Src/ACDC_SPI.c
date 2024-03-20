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
#include "ACDC_GPIO.h"
#include "ACDC_CLOCK.h"

#pragma region PRIVATE_FUNCTION_PROTOTYPES
/// @brief Enables the SPIx peripheral clock (Needed for peripheral to function)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
static void SPI_InitClk(const SPI_TypeDef *SPIx);

/// @brief Initilizes the various pins for SPI {See RM-181, DS-30}
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster true if SPIx should act as the master, false if it should act as a slave
static void SPI_InitPin(const SPI_TypeDef *SPIx, bool isMaster);
#pragma endregion

void SPI_InitCS(SPI_TypeDef *SPIx, bool isMaster, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
     SPI_InitClk(SPIx);
     SPI_InitPin(SPIx, isMaster);

    // Configure SPIx
    CLEAR_BIT(SPIx->CR1, SPI_CR1_SPE);              // Disable SPIx (so it can be configured)
    SPI_EnableSoftwareCS(SPIx, GPIOx, GPIO_PIN);    // Enable the software CS pin
    SPI_SetClockPhaseAndPolarity(SPIx, 0, 0);       // 1st Clk transmission is when data is captured, and Clk idles at 0
    SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_2);       // Set SPIx to the fastest speed
    SPI_SetBitMode(SPIx, SPI_MODE_16Bit);           // Set SPIx to 16 bit mode
    SPI_SetToMaster(SPIx, isMaster);                // Set to Master or Slave
    SPI_SetLsbFirst(SPIx, false);                   // Set it to MsbFirst
    SET_BIT(SPIx->CR1, SPI_CR1_SPE);                // Enable SPIx
}

void SPI_EnableSoftwareCS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    GPIO_PinDirection(GPIOx, GPIO_PIN, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_PUSH_PULL);    // Setup Software CS
    SET_BIT(SPIx->CR1, SPI_CR1_SSM | SPI_CR1_SSI);    // Enable Software Slave & set NSS low
}

void SPI_EnableRemap(SPI_TypeDef *SPIx, bool enable){
    if(SPIx == SPI1){
        if(enable){
            SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);  // Enable the Alternate Function Clk
            SET_BIT(AFIO->MAPR, AFIO_MAPR_SPI1_REMAP);  // Enable Remapping SPI1
        }
        else
            CLEAR_BIT(AFIO->MAPR, AFIO_MAPR_SPI1_REMAP); // Disable SPI1 Remapping
    }
    //No remap available for SPI2
}

void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data) {
    // Wait until transmit buffer is empty
    while (!(SPIx->SR & SPI_SR_TXE));

    // Send data
    SPIx->DR = data;

    while(!READ_BIT(SPIx->SR, SPI_SR_TXE)){}    // Wait to return until the transmission has completed (Needed for CS pin)
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

uint16_t SPI_TransmitReceiveCS(SPI_TypeDef *SPIx, uint16_t data, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    GPIO_Clear(GPIOx, GPIO_PIN);                                // Set CS Low
    uint16_t returnedData = SPI_TransmitReceive(SPIx, data);    // Transmit and Recieve the data
    while(READ_BIT(SPIx->SR, SPI_SR_BSY)){}                     // Wait until the data has finished sending
    GPIO_Set(GPIOx, GPIO_PIN);                                  // Set CS High
    return returnedData;                                        // Return the SPI data
}

void SPI_SetBaudDivider(SPI_TypeDef *SPIx, SPI_BaudDivider SPI_BAUD_DIV_x){
    while(READ_BIT(SPIx->SR, SPI_SR_BSY)){}                 // While SPIx is busy in communication or Tx buffer is not empty
    CLEAR_BIT(SPIx->CR1, SPI_CR1_BR_Msk);                   // Clear the SPIx Baud Divisor bits
    SET_BIT(SPIx->CR1, SPI_BAUD_DIV_x << SPI_CR1_BR_Pos);   // Set the SPIx Baud Divisor bits
}

void SPI_CalculateAndSetBaudDivider(SPI_TypeDef *SPIx, uint32_t maxPeripheralClockSpeed){
    uint32_t SpiClockSpeed;
    if(SPIx == SPI1)
        SpiClockSpeed = CLOCK_GetAPB2ClockSpeed(); // SPI1 is on the APB2 Clock {See RM-113} (MAX = 72MHz)
    else // SPI2
        SpiClockSpeed = CLOCK_GetAPB1ClockSpeed(); // SPI2 is on the APB1 Clock {See RM-116} (MAX = 36MHz)

    if(SpiClockSpeed / 2 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_2);
    else if(SpiClockSpeed / 4 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_4);
    else if(SpiClockSpeed / 8 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_8);
    else if(SpiClockSpeed / 16 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_16);
    else if(SpiClockSpeed / 32 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_32);
    else if(SpiClockSpeed / 64 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_64);
    else if(SpiClockSpeed / 128 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_128);
    else if(SpiClockSpeed / 256 <= maxPeripheralClockSpeed)
        SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_256);
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

void SPI_SetClockPhaseAndPolarity(SPI_TypeDef *SPIx, bool ClkPhase, bool ClkPolarity){
    if(ClkPhase)                                
        SET_BIT(SPIx->CR1  , SPI_CR1_CPHA);     // The second clock transition is the first data capture edge
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_CPHA);     // The first clock transition is the first data capture edge

    if(ClkPolarity)
        SET_BIT(SPIx->CR1  , SPI_CR1_CPOL);     // Set Clock to 1 or High when idle
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_CPOL);     // Set Clock to 0 or Low when idle
}

void SPI_SetToMaster(SPI_TypeDef *SPIx, bool isMaster){
    if(isMaster)
        SET_BIT(SPIx->CR1, SPI_CR1_MSTR); // Set as master, (Default: Slave)
    else
        CLEAR_BIT(SPIx->CR1, SPI_CR1_MSTR);
}

bool SPI_HasDataToRecieve(SPI_TypeDef *SPIx){
    return READ_BIT(SPIx->SR, SPI_SR_RXNE) ? true : false; // Checks if the SPIx's recieve buffer is not empty
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