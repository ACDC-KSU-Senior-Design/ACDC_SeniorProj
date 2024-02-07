/**
 * @file ACDC_SPI.h
 * @author Liam Bramley, Devin Marx
 * @brief Header file for SPI driver
 * @version 0.1
 * @date 2024-02-06
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_SPI_H
#define __ACDC_SPI_H

#include "stm32f1xx.h"
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

/// @brief Initializes the SPIx peripheral to either the master or slave. (Default Values: SPI_MODE_8Bit, SPI_BAUD_DIV_2, MSB First,  )
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster True if SPIx should act as the master, false if it should act as the slave
void SPI_Init(SPI_TypeDef *SPIx, bool isMaster);

/// @brief Changes the Baud rate divider of the SPIx peripheral
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param SPI_BAUD_DIV_x SPI Tx Baud rate divider (Ex. SPI_BAUD_DIV_2, SPI_BAUD_DIV_16, ...)
void SPI_SetBaudDivider(SPI_TypeDef *SPIx, SPI_BaudDivider SPI_BAUD_DIV_x);

/// @brief Changes the SPIx peripheral between 8-bit and 16-bit mode
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param SPI_MODE_x Bit mode selection for SPIx (Ex. SPI_MODE_8bit or SPI_MODE_16bit)
void SPI_SetBitMode(SPI_TypeDef *SPIx, SPI_BitMode SPI_MODE_x);

/// @brief Sets SPIx into either MsbFirst or LsbFirst mode.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param LsbFirst True if the data format should be LsbFirst, else false for MsbFirst.
void SPI_SetLsbFirst(SPI_TypeDef *SPIx, bool LsbFirst);

/// @brief Sets the clock phase and polarity for SPIx.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param ClkPhase True for second clock transition as first data capture edge, False for first clock transition. {See RM-744}
/// @param ClkPolarity True to set clock to 1 when idle, else false to set low while idle.
void SPI_SetClockPhaseAndPolarity(SPI_TypeDef *SPIx, bool ClkPhase, bool ClkPolarity);

/// @brief Configures SPIx peripheral as master or slave mode.
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param isMaster True to configure SPI as master, False to configure it as slave.
void SPI_SetToMaster(SPI_TypeDef *SPIx, bool isMaster);

/// @brief Transmits data over SPIx
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param data Data to transmit, either 8 or 16 bit (Default: 8-bit).
void SPI_Transmit(SPI_TypeDef *SPIx, uint16_t data);

/// @brief Recieves data from SPIx
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @return Data recieved from SPIx, either 8 or 16 bit (Default: 8-bit).
uint16_t SPI_Receive(SPI_TypeDef *SPIx);

/// @brief Performs a transmit and then a Recieve (BLOCKING) 
/// @param SPIx SPI Peripheral (Ex. SPI1 or SPI2)
/// @param data Data to transmit, either 8 or 16 bit (Default: 8-bit).
/// @return ata recieved from SPIx, either 8 or 16 bit (Default: 8-bit).
uint16_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint16_t data); //TODO: Should probably look into Tx/Rx simultaneously

#endif