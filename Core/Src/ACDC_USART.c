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

static uint8_t USART_Initialized = 0;

#pragma region PRIVATE_FUNCTION_PROTOTYPES
/// @brief Enables the USARTx peripheral clock (Needed for peripheral to function)
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
static void USART_InitClk(const USART_TypeDef *USARTx);

/// @brief Initilizes the various pins for UART/USART {See RM-167, RM-168}
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @param useUART true if you want UART, false if you want USART
static void USART_InitPin(const USART_TypeDef *USARTx, bool useUART);

/// @brief Calculates the USARTDIV for the USARTx-BRR using the SerialSpeed Serial_x and the system clock speed
/// @param Serial_x Tx/Rx speed of the USART peripheral (Ex. Serial_115200, Serial_9600, ...)
/// @return USARTDIV to be stored in the USARTx->BRR Register
static uint16_t USART_CalculateUSARTDIV(SerialSpeed Serial_x);

/// @brief Checks if the current USARTx peripheral has been initialized.
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
/// @return True if the current USARTx peripheral has been initialized, false otherwise.
static bool USART_IsInitialized(const USART_TypeDef *USARTx);

/// @brief Sets the initialization status of the current USARTx peripheral.
/// @param USARTx USART Peripheral (Ex. USART1, USART2, ...)
static void USART_SetInitialized(const USART_TypeDef *USARTx);
#pragma endregion

#pragma region PUBLIC_FUNCTIONS
void USART_Init(USART_TypeDef *USARTx, SerialSpeed Serial_x, bool useUART){
    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_AFIOEN);  // Enable the Clock for Alternate Functions
    SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN );  // Enable the Power Interface (Unsure of function)
    USART_InitClk(USARTx);                      // Enables the USARTx Clock on the APBx Register
    USART_InitPin(USARTx, useUART);             // Sets up the USARTx pins to be used as Input and Output.

    CLEAR_BIT(USARTx->CR1, USART_CR1_UE);                       // Disable the USART peripheral

    USART_ChangeSerialSpeed(USARTx, Serial_x);                              // Calculate and modify the BRR register
    SET_BIT(USARTx->CR1, USART_CR1_TE | USART_CR1_RE);                      // Enable the Transmitter and Reciever
    CLEAR_BIT(USARTx->CR1, USART_CR1_M | USART_CR1_PCE | USART_CR1_RWU);    // Word Length = 8, Disable Parity Control, Enable Rx Active Mode
    CLEAR_BIT(USARTx->CR2, USART_CR2_STOP);                                 // 1 Stop Bit

    SET_BIT(USARTx->CR1, USART_CR1_UE);                         // Enable the USART peripheral

    SetInitilized(USARTx);
}

void USART_ChangeSerialSpeed(USART_TypeDef *USARTx, SerialSpeed Serial_x){
    if(USART_IsInitilized(USARTx))                                    // If the preherphial has already been initilized
        CLEAR_BIT(USARTx->CR1, USART_CR1_UE);                   // Disable the USART peripheral
    
    WRITE_REG(USARTx->BRR, USART_CalculateUSARTDIV(Serial_x));  // Set the Baud Rate 

    if(USART_IsInitilized(USARTx))                                    // If the preherphial has already been initilized
        SET_BIT(USARTx->CR1, USART_CR1_UE);                     // Enable the USART peripheral
}

void USART_SendChar(USART_TypeDef *USARTx, char chr){
    while(!READ_BIT(USARTx->SR, USART_SR_TXE)){}  // Wait until buffer is ready to transmit again
    WRITE_REG(USARTx->DR, chr & USART_DR_DR_Msk); // Transmit the data
}

void USART_SendString(USART_TypeDef *USARTx, char* str){
    for(uint32_t i = 0; str[i] != '\0'; i++)    // Iterate over the whole string
        USART_SendChar(USARTx, str[i]);         // Send each character 1 by 1

    USART_SendChar(USARTx, '\r'); // Terminate the USART Transaction
    USART_SendChar(USARTx, '\n'); // Carriage return & Line feed
}

char USART_RecieveChar(USART_TypeDef *USARTx){
    while(!READ_BIT(USARTx->SR, USART_SR_RXNE)){}  // Wait until available in the buffer
    return READ_REG(USARTx->DR & 0xFF);            // Retrieve the character and return it
}

void USART_RecieveString(USART_TypeDef *USARTx, char* buffer, uint16_t bufferLen){
    uint16_t i;
    for(i = 0; i < bufferLen; i++){                    // Iterate over the maximum buffer size
        buffer[i] = USART_RecieveChar(USARTx);         // Read the next character in the USARTx buffer
        if(buffer[i] == '\n' && buffer[i-1] == '\r')   // If the null terminating character is detected
            break;                                     // Break out of the loop
    }

    if(buffer[i] != '\n')                              // If more characters are sent than the buffer can store
        while(USART_RecieveChar(USARTx) != '\n'){}     // Read the characters until there are no more to be recieved

    buffer[i-1] = '\0';                                // End the string with a null terminating character
}

bool USART_HasDataToRecieve(USART_TypeDef *USARTx){
    return READ_BIT(USARTx->SR, USART_SR_RXNE) >> USART_SR_RXNE_Pos;
}
#pragma endregion

#pragma region PRIVATE_FUNCTIONS
static void USART_InitClk(const USART_TypeDef *USARTx){
    if(USARTx == USART1)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_USART1EN);    //Enable USART1 Clock
    else if(USARTx == USART2)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART2EN);    //Enable USART2 Clock
    else if(USARTx == USART3)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_USART3EN);    //Enable USART3 Clock
}

static void USART_InitPin(const USART_TypeDef *USARTx, bool useUART){
    GPIO_TypeDef *GPIO_Port = GPIOA;
    uint16_t Tx_Pin=0, Rx_Pin=0, Ck_Pin, CTS_Pin, RTS_Pin;
    
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
            GPIO_PinDirection(GPIO_Port, CTS_Pin, GPIO_MODE_AF_INPUT          , GPIO_CNF_INPUT_FLOATING     );
            GPIO_PinDirection(GPIO_Port, RTS_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);
        }
    } 
    //                          GPIO Configuration {See RM-166, RM-167}
    GPIO_PinDirection(GPIO_Port, Tx_Pin, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);    
    GPIO_PinDirection(GPIO_Port, Rx_Pin, GPIO_MODE_INPUT             , GPIO_CNF_INPUT_FLOATING     );
}

static uint16_t USART_CalculateUSARTDIV(SerialSpeed Serial_x){
    SystemClockSpeed SCS_x = CLOCK_GetSystemClockSpeed();  // Grab the Current System Clock Speed
    uint16_t Mantissa = SCS_x / (16 * Serial_x);           // Calculate the Mantissa
    uint8_t Divisor = (SCS_x / Serial_x) % 16;             // Calculate the Divider               {See RM-820}
    return (Mantissa << 4) & 0xFFF0 | (Divisor & 0xF);     // Calculate and return the USARTDIV   {See RM-789}
}

static bool USART_IsInitialized(const USART_TypeDef *USARTx){
    if(USARTx == USART1)
        return READ_BIT(USART_Initialized, 0b001);
    else if(USARTx == USART2)
        return READ_BIT(USART_Initialized, 0b010);
    else if(USARTx == USART3)
        return READ_BIT(USART_Initialized, 0b100);
    else
        return false;
}       

static void USART_SetInitialized(const USART_TypeDef *USARTx){
    if(USARTx == USART1)
        SET_BIT(USART_Initialized, 0b001);
    else if(USARTx == USART2)
        SET_BIT(USART_Initialized, 0b010);
    else if(USARTx == USART3)
        SET_BIT(USART_Initialized, 0b100);
}
#pragma endregion