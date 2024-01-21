/**
 * @file ACDC_Assert.c
 * @author Devin Marx
 * @brief Impementation file for the assertion handler and related functions
 * @version 0.1
 * @date 2024-01-21
 * 
 * @copyright Copyright (c) 2024
 */

#include "ACDC_Assert.h"
#include "ACDC_stdbool.h"
#include "stm32f1xx.h"

#define BREAKPOINT() asm volatile("BKPT") // Macro that triggers a software breakpoint

void AssertHandler(void){

    BREAKPOINT(); // Triggers a breakpoint in software

    SET_BIT(RCC->APB2ENR, RCC_APB2ENR_IOPAEN);                      // Enable GPIOA Prepherial Clock
    CLEAR_BIT(GPIOA->CRL, GPIO_CRL_MODE5_Msk | GPIO_CRL_CNF5_Msk);  // Reset the CNF and MODE bits for GPIOA, PIN_5 (Sets Mode as Output PP)
    SET_BIT(GPIOA->CRL, GPIO_CRL_MODE5_1);                          // Set the MODE bit to OUTPUT 2Mhz
    
    while(true){
        //DELAY 500
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BR5); // Turn off the LED
        //DELAY 500 
        SET_BIT(GPIOA->BSRR, GPIO_BSRR_BS5); // Turn on the LED
    }
    //Green LED   => GPIOA, GPIO_PIN_5
}