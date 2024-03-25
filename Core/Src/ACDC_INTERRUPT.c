/**
 * @file ACDC_INTERRUPT.c
 * @brief Implementation of general-purpose interrupt configuration functions.
 * 
 * This file provides functions for configuring and managing interrupts, including
 * support for GPIO interrupts.
 * 
 * @author Devin Marx
 * @version 0.1
 * @date 2023-11-02
 * 
 * @copyright Copyright (c) 2023-2024
 * 
 * The functions in this file enable the configuration of interrupts, allowing
 * users to set up interrupt triggers on various sources, including GPIO pins.
 *
 * @note External interrupts are muxed, allowing the configuration of up to 16
 *       concurrent interrupts. Refer to RM-210 for details.
 */

#include "ACDC_INTERRUPT.h"
#include "ACDC_GPIO.h"

void INTERRUPT_Enable(IRQn_Type IRQn){
    // Enables Specific Interrupt Vectors {See PM-120}
    if(IRQn >= 0){
        uint8_t index = IRQn >> 5;                      // Get the index of the IRQ {0-31 = 0, 32-63 = 1}
        uint8_t irqToEnable = 0b1 << (IRQn & 0x1F);     // Shift the set bit down 0-31 places
        SET_BIT(NVIC->ISER[index], irqToEnable);        // Set the bit to enable the IRQn interrupt vector
    }
}

void INTERRUPT_Disable(IRQn_Type IRQn){
    // Disables Specific Interrupt Vectors {See PM-121}
    if(IRQn >= 0){
        uint8_t index = IRQn >> 5;                      // Get the index of the IRQ {0-31 = 0, 32-63 = 1}
        uint8_t irqToDisable = 0b1 << (IRQn & 0x1F);    // Shift the set bit down 0-31 places
        SET_BIT(NVIC->ICER[index], irqToDisable);       // Set the bit to disable the IRQn interrupt vector
    }
}

void INTERRUPT_SetPriority(IRQn_Type IRQn, uint8_t Priority){
    // IRQn values >= 0 are STM32 specific interrupt vectors
    // IRQn values < 0 are Cortex-M3 specific interrupt vectors
    // IRQn values -13 & -14 cannot change their priorities (HardFault_IRQn , NonMaskableInt_IRQn) {See PM-33}

    const uint8_t STM32_LOWEST_INT_VECTOR = WWDG_IRQn;                              // Lowest STM32 specific interrupt vector
    const uint8_t CORTEX_M3_SCB_SHP_OFFSET = 12;                                    // Offset used to convert a interrupt vector into a SCB->SHPR index. {See PM-138, PM-139, PM-140} 
    const int8_t CORTEX_M3_LOWEST_CONFIGURABLE_INT_VECTOR = MemoryManagement_IRQn;  // Lowest configurable Cortex-M3 vector
    uint8_t calcPriority = (Priority << 4) & 0xFF;                                  // The processor implements bits [7:4], bits [3:0] are 0 and ignore writes {See PM-125}

    if(IRQn >= STM32_LOWEST_INT_VECTOR)                                     // If it is a STM32 specific interrupt vector
        WRITE_REG(NVIC->IP[IRQn], calcPriority);                            // Write the calculated priority to the register
    else if(IRQn >= CORTEX_M3_LOWEST_CONFIGURABLE_INT_VECTOR)               // If it is a configurable Cortex-M3 interrupt vector
        WRITE_REG(SCB->SHP[IRQn + CORTEX_M3_SCB_SHP_OFFSET], calcPriority); // Write the calcualted priority to the SCB->SHP register
}


void GPIO_INT_SetToInterrupt(const GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, GPIO_TriggerType TT_x){
    //External Interrupts are Muxed so you can only use 16 concurrently {See Page 210 RM}
    //https://community.st.com/t5/stm32-mcus-products/nvic-interrupts-on-two-ports-with-same-pin-number/td-p/550469

    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // Enable Alternate Functions (Needed for Interrupts)

    uint16_t pin = GPIO_GetPinNumber(GPIO_PIN);
    EXTI->IMR |= 1 << pin;        // Enable Interrupts on pin
    if(TT_x & TT_RISING_EDGE)
        EXTI->RTSR |= 1 << pin;   // Enable RisingEdge Interrupts on pin
    if(TT_x & TT_FALLING_EDGE)
        EXTI->FTSR |= 1 << pin;   // Enable FallingEdge Interrupts on pin

    uint16_t CRNumber = pin >> 2; // Gets the EXTICRx number
    uint16_t GpioMask = 0b0000;   // default to GPIOA (GPIO mask {See RM-191})
    if(GPIOx == GPIOB)
        GpioMask = 0b0001;
    else if(GPIOx == GPIOC)
        GpioMask = 0b0010;
    else if(GPIOx == GPIOD)
        GpioMask = 0b0011;
    else if(GPIOx == GPIOE)
        GpioMask = 0b0100;

    AFIO->EXTICR[CRNumber] &= ~(0b1111 << (pin % 4)*4); //Clear Register First 
    AFIO->EXTICR[CRNumber] |= GpioMask << (pin % 4)*4;  //Set Register to Value  
}