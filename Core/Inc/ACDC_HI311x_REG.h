/**
 * @file ACDC_HI311x_REG.h
 * @author Devin Marx
 * @brief Registers and their values for the ARINC 825 HI311x chips
 * @version 0.1
 * @date 2024-03-04
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_HI311x_REG_H
#define __ACDC_HI311x_REG_H

typedef enum{               /* Write-Only Registers for HI311x Chips  */
    WRITE_CTRL0  = 0x14,    /**< Write Control Register 0             */
    WRITE_CTRL1  = 0x16,    /**< Write Control Register 1             */
    WRITE_BTR0   = 0x18,    /**< Write Bit Timing Register 0          */
    WRITE_BTR1   = 0x1A,    /**< Write Bit Timing Register 1          */
    WRITE_TEC    = 0x26,    /**< Transmit Error Counter Register      */
    WRITE_REC    = 0x24,    /**< Receive Error Counter Register       */
    WRITE_INTE   = 0x1C,    /**< Interrupt Enable Register            */
    WRITE_STATFE = 0x1E,    /**< Status Flag Enable Register          */
    WRITE_GPINE  = 0x22     /**< General Purpose Pins Enable Register */
}HI3110_WriteRegisters;     // For Register Op-Code {See HI3110-16}

typedef enum{               /* Read-Only Registers for HI311x Chips          */
    READ_CTRL0   = 0xD2,    /**< Read Control Register 0                     */
    READ_CTRL1   = 0xD4,    /**< Read Control Register 1 					 */
    READ_BTR0  	 = 0xD6,    /**< Read Bit Timing Register 0 				 */
    READ_BTR1  	 = 0xD8,    /**< Read Bit Timing Register 1 				 */
    READ_TEC  	 = 0xEC,    /**< Read Transmit Error Counter Register 	     */
    READ_REC  	 = 0xEA,    /**< Read Receive Error Counter Register 		 */
    READ_MESSTAT = 0xDA,    /**< Read Message Status Register 			     */
    READ_ERR 	 = 0xDC,    /**< Read Error Register 						 */
    READ_INTF 	 = 0xDE,    /**< Read Interrupt Flag Register				 */
    READ_INTE  	 = 0xE4,    /**< Read Interrupt Enable Register 			 */
    READ_STATF 	 = 0xE2,    /**< Read Status Flag Register 				     */
    READ_STATFE  = 0xE6,    /**< Read Status Flag Enable Register 		     */
    READ_GPINE   = 0xE8,    /**< Read General Purpose Pins Enable Register   */
    READ_TIMERUB = 0xFA,    /**< Read Free-Running Timer Upper Byte Register */
    READ_TIMERLB = 0xFA     /**< Read Free-Running Timer Lower Byte Register */
    //NOTE: Free-running counter registers. TIMERUB:TIMERLB are read with a single SPI Op-code (0xFA) as a 16-bit value in two SPI data bytes.
}HI3110_ReadRegisters;      // For register Op-Code {See HI3110-16}

/*****************  Bit definition for HI311x_CTRL0 register  *****************/
/* MODE configurations */
#define HI311x_CTRL0_MODE_Pos            (5u)
#define HI311x_CTRL0_MODE_Msk            (0b111u << HI311x_CTRL0_MODE_Pos) /** The HI311x_CTRL0_MODE Mask */
#define HI311x_CTRL0_MODE_Normal         (0b000u << HI311x_CTRL0_MODE_Pos) /** Normal Mode. Normal CAN operation. */
#define HI311x_CTRL0_MODE_Normal         (0b000u << HI311x_CTRL0_MODE_Pos) /** Normal Mode. Normal CAN operation. */
#define HI311x_CTRL0_MODE_Loopback       (0b001u << HI311x_CTRL0_MODE_Pos) /** Loopback Mode. The transceiver digital input is fed back to the receiver without disturbing the bus. This mode can be used for test purposes, allowing the HI-3110 to receive its own messages.*/
#define HI311x_CTRL0_MODE_Monitor        (0b010u << HI311x_CTRL0_MODE_Pos) /** Monitor Mode. The HI-3110 can be set up to monitor bus activity without transmitting to the bus (no ACK bits or error frames are sent in this mode). Receive filters can be programmed in Initialization Mode to buffer selected messages.*/
#define HI311x_CTRL0_MODE_Sleep          (0b011u << HI311x_CTRL0_MODE_Pos) /** Sleep Mode. The HI-3110 can be placed in a low power sleep mode if there is no bus activity and the transmit FIFO is empty. Sleep mode is exited by selecting an alternative mode of operation, or automatic wake up following bus activity can be enabled by setting the WAKEUP bit. The device will wake up in Monitor mode.*/
#define HI311x_CTRL0_MODE_Initialization (0b100u << HI311x_CTRL0_MODE_Pos) /** Initialization Mode. The device must be in this mode for bit timing and filter set-up. This is the default following reset. The host exits initialization mode by selecting an alternative mode of operation.*/

/* WEKUP configurations */
#define HI311x_CTRL0_WAKEUP_Pos (4u)
#define HI311x_CTRL0_WAKEUP_Msk (0b1u << HI311x_CTRL0_WAKEUP_Pos) /** The HI311x_CTRL0_WAKEUP Mask */
#define HI311x_CTRL0_WAKEUP     (0b1u << HI311x_CTRL0_WAKEUP_Pos) /** When this bit is set, the HI-3110 will automatically wake up from Sleep Mode to Monitor Mode when it detects activity on the bus. */

/* RESET configurations */
#define HI311x_CTRL0_RESET_Pos (3u)
#define HI311x_CTRL0_RESET_Msk (0b1u << HI311x_CTRL0_RESET_Pos) /** The HI311x_CTRL0_RESET Mask */
#define HI311x_CTRL0_RESET     (0b1u << HI311x_CTRL0_RESET_Pos) /** Setting this bit causes HI-3110 reset to occur. Following reset, the CTRL0register should be written with < 1 x x x 0 x x x >. This will clear the RESET bit and also avoid unpredictable behavior by ensuring the part is programmed to Initialization Mode, ready for set-up. */

/* BOR configurations */
#define HI311x_CTRL0_BOR_Pos (2u)
#define HI311x_CTRL0_BOR_Msk (0b1u << HI311x_CTRL0_BOR_Pos) /** The HI311x_CTRL0_BOR Mask */
#define HI311x_CTRL0_BOR     (0b1u << HI311x_CTRL0_BOR_Pos) /** Bus-off Reset. When this bit is set, automatic bus-off recovery is initiated following 128 occurrences of 11 consecutive recessive bits on the bus. The HI-3110 will become error-active with both its error counters set to zero and resume operation in Normal Mode. */

/* TDIV configurations */
#define HI311x_CTRL0_TDIV_Pos  (0u)
#define HI311x_CTRL0_TDIV_Msk  (0b11u << HI311x_CTRL0_TDIV_Pos) /** The HI311x_CTRL0_TDIV Mask               */
#define HI311x_CTRL0_TDIV_DIV1 (0b00u << HI311x_CTRL0_TDIV_Pos) /** No division (counts every bit clock)    */
#define HI311x_CTRL0_TDIV_DIV2 (0b01u << HI311x_CTRL0_TDIV_Pos) /** Divide by 2 (counts every 2 bit clocks) */
#define HI311x_CTRL0_TDIV_DIV4 (0b10u << HI311x_CTRL0_TDIV_Pos) /** Divide by 4 (counts every 4 bit clocks) */
#define HI311x_CTRL0_TDIV_DIV8 (0b11u << HI311x_CTRL0_TDIV_Pos) /** Divide by 8 (counts every 8 bit clocks) */
#define HI311x_CTRL0_TDIV_0    (0b01u << HI311x_CTRL0_TDIV_Pos) /** TDIV Bit 0 */
#define HI311x_CTRL0_TDIV_1    (0b10u << HI311x_CTRL0_TDIV_Pos) /** TDIV Bit 1*/

/*****************  Bit definition for HI311x_CTRL1 register  *****************/
/* TXEN configurations */
#define HI311x_CTRL1_TXEN_Pos (7u)
#define HI311x_CTRL1_TXEN_Msk (0b1u << HI311x_CTRL1_TXEN_Pos) /** The HI311x_TXEN Mask */
#define HI311x_CTRL1_TXEN     (0b1u << HI311x_CTRL1_TXEN_Pos) /** Transmission Enable. This bit is logically ORed with the TXEN pin. When this bit is asserted, each message in the FIFO will be sequentially loaded to the transmit buffer and sent if the bus is available. If this bit is not set, a transmission can be enabled by either the TXEN pin or the TX1M bit. If the TXEN pin is pulled low during a transmission, the current message being transmitted will be completed. Any additional messages in the FIFO will not be transmitted. */

/* TX1M configurations */
#define HI311x_CTRL1_TX1M_Pos (6u)
#define HI311x_CTRL1_TX1M_Msk (0b1u << HI311x_CTRL1_TX1M_Pos) /** The HI311x_TX1M Mask */
#define HI311x_CTRL1_TX1M     (0b1u << HI311x_CTRL1_TX1M_Pos) /** Enable transmission of only next message. This bit is applicable only if TXEN = 0. It is reset automatically upon completion of a successful transmission or by initiation of transmission if the OSM bit is set. */

/* OSM configurations */
#define HI311x_CTRL1_OSM_Pos (5u)
#define HI311x_CTRL1_OSM_Msk (0b1u << HI311x_CTRL1_OSM_Pos) /** The HI311x_OSM Mask */
#define HI311x_CTRL1_OSM     (0b1u << HI311x_CTRL1_OSM_Pos) /** One-Shot Mode Enable. OSM is intended to be used ONLY with the TX1M bit. If OSM is enabled and TX1M is set, the controller transmits only once and does not attempt re-transmission upon loss of arbitration or error. This feature is necessary to support the implementation of fixed time slots in the Time-Triggered CAN standard (TTCAN). */
// NOTE: Un-transmitted messages will remain in the FIFO. If a new message is required on the next transmission cycle, the user must first clear the FIFO with SPI command 0x54 and then reload the new message.

/* FILTON configurations */
#define HI311x_CTRL1_FILTON_Pos (4u)
#define HI311x_CTRL1_FILTON_Msk (0b1u << HI311x_CTRL1_FILTON_Pos) /** The HI311x_FILTON Mask */
#define HI311x_CTRL1_FILTON     (0b1u << HI311x_CTRL1_FILTON_Pos) /** Filter on enable. This bit is set to turn on the HI-3110 CAN ID filtering mechanism. The default after reset is FILTON = 0, meaning filtering is turned off and every valid CAN message is accepted into the receive FIFO */
// NOTE: Note: The device must be in initialization mode in order to program the acceptance filters and masks. 

/* OSCOFF configurations */
#define HI311x_CTRL1_OSCOFF_Pos (3u)
#define HI311x_CTRL1_OSCOFF_Msk (0b1u << HI311x_CTRL1_OSCOFF_Pos) /** The HI311x_OSCOFF Mask */
#define HI311x_CTRL1_OSCOFF     (0b1u << HI311x_CTRL1_OSCOFF_Pos) /** Oscillator off. This bit should be set to a one if an external clock is used. In this case the external clock is connected to the OSCIN pin and OSCOUTshould be left floating. */

/* CLKDIV configurations */
#define HI311x_CTRL1_CLKDIV_Pos  (0u)
#define HI311x_CTRL1_CLKDIV_Msk  (0b11u << HI311x_CTRL1_CLKDIV_Pos) /** The HI311x_CLKDIV Mask      */
#define HI311x_CTRL1_CLKDIV_DIV1 (0b00u << HI311x_CTRL1_CLKDIV_Pos) /** Divide External CLKOUT by 1 */
#define HI311x_CTRL1_CLKDIV_DIV2 (0b01u << HI311x_CTRL1_CLKDIV_Pos) /** Divide External CLKOUT by 2 */
#define HI311x_CTRL1_CLKDIV_DIV4 (0b10u << HI311x_CTRL1_CLKDIV_Pos) /** Divide External CLKOUT by 4 */
#define HI311x_CTRL1_CLKDIV_DIV8 (0b11u << HI311x_CTRL1_CLKDIV_Pos) /** Divide External CLKOUT by 8 */
#define HI311x_CTRL1_CLKDIV_0    (0b01u << HI311x_CTRL1_CLKDIV_Pos) /** CLKDIV Bit 0                */
#define HI311x_CTRL1_CLKDIV_1    (0b10u << HI311x_CTRL1_CLKDIV_Pos) /** CLKDIV Bit 1                */

/*****************  Bit definition for HI311x_BTR0 register  *****************/
/* SJW1 configurations */
#define HI311x_BTR0_SJW_Pos (6u)
#define HI311x_BTR0_SJW_Msk (0b11u << HI311x_BTR0_SJW_Pos) /** The HI311x_SJW Mask                                 */
#define HI311x_BTR0_SJW_1Tq (0b00u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width bits = 1 time quantum */
#define HI311x_BTR0_SJW_2Tq (0b01u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width bits = 2 time quantum */
#define HI311x_BTR0_SJW_3Tq (0b10u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width bits = 3 time quantum */
#define HI311x_BTR0_SJW_4Tq (0b11u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width bits = 4 time quantum */
#define HI311x_BTR0_SJW_0   (0b01u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width Bit 0                 */
#define HI311x_BTR0_SJW_1   (0b10u << HI311x_BTR0_SJW_Pos) /** Re-synchronization Jump Width Bit 1                 */

/* BRP configurations */
#define HI311x_BTR0_BRP_Pos (0u)
#define HI311x_BTR0_BRP_Msk (0b111111 << HI311x_BTR0_BRP_Pos) /** The HI311x_BRP Mask       */
#define HI311x_BTR0_BRP_0   (0b000001 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 0 */
#define HI311x_BTR0_BRP_1   (0b000010 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 1 */
#define HI311x_BTR0_BRP_2   (0b000100 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 2 */
#define HI311x_BTR0_BRP_3   (0b001000 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 3 */
#define HI311x_BTR0_BRP_4   (0b010000 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 4 */
#define HI311x_BTR0_BRP_5   (0b100000 << HI311x_BTR0_BRP_Pos) /** Baud Rate Prescaler Bit 5 */

#endif