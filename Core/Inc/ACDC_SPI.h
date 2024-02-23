/**
 * @file ACDC_SPI.h
 * @author Liam Bramley, Devin Marx
 * @brief Header file for SPI driver
 * @version 0.1
 * @date 2024-01-30
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_SPI_H
#define __ACDC_SPI_H

#include "stm32f1xx.h"
#include "ACDC_GPIO.h"
#include "ACDC_stdbool.h"

typedef enum{ // SPI Baud Rate Divider
    SPI_BAUD_DIV_2   = 0b000,   /**< Baud rate: SysClk / 2 bps   */
    SPI_BAUD_DIV_4   = 0b001,   /**< Baud rate: SysClk / 4 bps   */
    SPI_BAUD_DIV_8   = 0b010,   /**< Baud rate: SysClk / 8 bps   */
    SPI_BAUD_DIV_16  = 0b011,   /**< Baud rate: SysClk / 16 bps  */
    SPI_BAUD_DIV_32  = 0b100,   /**< Baud rate: SysClk / 32 bps  */
    SPI_BAUD_DIV_64  = 0b101,   /**< Baud rate: SysClk / 64 bps  */
    SPI_BAUD_DIV_128 = 0b110,   /**< Baud rate: SysClk / 128 bps */
    SPI_BAUD_DIV_256 = 0b111    /**< Baud rate: SysClk / 256 bps */
}SPI_BaudDivider;

typedef enum{   // SPI Data Frame Format
    SPI_MODE_8Bit  = 0,         /**< 8-bit data frame format for Tx/Rx  */
    SPI_MODE_16Bit = 1          /**< 16-bit data frame format for Tx/Rx */
}SPI_BitMode;

/// @brief Initializes the SPIx peripheral to either the master or slave. (Default Values: SPI_MODE_16Bit, SPI_BAUD_DIV_2, MSB First)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster True if SPIx should act as the master, false if it should act as the slave
void SPI_Init(SPI_TypeDef *SPIx, bool isMaster);

/// @brief Enables pin remapping on SPIx depending on the value of enable (Only SPI1 can be remapped)
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param enable True if the system should enable remapping for SPIx, false if it should not remap SPIx
void SPI_EnableRemap(SPI_TypeDef *SPIx, bool enable);

/// @brief Transmits data over the given SPI
/// @param SPIx SPI to transmit over
/// @param data Data to transmit
void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data);

/// @brief Recieves data from a SPI
/// @param SPIx SPI to recieve from
/// @return Data recieved
uint16_t SPI_Receive(const SPI_TypeDef *SPIx);

/// @brief Transmits and recieves data over the SPI
/// @param SPIx SPI to transmit over
/// @param data Data to transmit
/// @return Data recieved
uint16_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint16_t data);

/// @brief Changes the Baud rate divider of the SPIx peripheral
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param SPI_BAUD_DIV_x SPI Tx Baud rate divider (Ex. SPI_BAUD_DIV_2, SPI_BAUD_DIV_4, ...)
void SPI_SetBaudDivider(SPI_TypeDef *SPIx, SPI_BaudDivider SPI_BAUD_DIV_x);

/// @brief Calculates and sets the SPI baud divider to accompany the current peripherals maximum clock speed
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param maxPeripheralClockSpeed Maximum clock speed the SPI peripheral can run at
void SPI_CalculateAndSetBaudDivider(SPI_TypeDef *SPIx, uint32_t maxPeripheralClockSpeed);

/// @brief Changes the SPIx peripheral between 8-bit and 16-bit mode
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param SPI_MODE_x Bit mode selection for SPIx (Ex. SPI_MODE_8bit or SPI_MODE_16bit)
void SPI_SetBitMode(SPI_TypeDef *SPIx, SPI_BitMode SPI_MODE_x);

/// @brief Sets SPIx into either MsbFirst or LsbFirst mode.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param LsbFirst True if the data format should be LsbFirst, else false for MsbFirst.
void SPI_SetLsbFirst(SPI_TypeDef *SPIx, bool LsbFirst);
#endif