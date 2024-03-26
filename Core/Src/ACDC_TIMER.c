/**
 * @file ACDC_TIMER.c
 * @author Devin Marx
 * @brief Implementation of timer functions for system clock speed and delay
 * @version 0.1
 * @date 2023-11-7
 * 
 * @copyright Copyright (c) 2023-2024
 */

#include "ACDC_TIMER.h"
#include "ACDC_INTERRUPT.h"
#include "stm32f1xx.h"

#define MS_PER_SECOND 1000  // Number of milliseconds per second
#define US_PER_MS     1000  // Number of microseconds per millisecond

volatile static uint64_t SysTickCounter;
static uint8_t SCS_IN_MHz;                  // Clock frequency in MHz (SCS_72Mhz -> 72, SCS_36Mhz -> 36, Etc.)

void TIMER_Init(SystemClockSpeed SCS_x){  //TODO: Should only ever be called once (Need to add an assert)
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
    TIMER_SetSystemClockSpeed(SCS_x);
    INTERRUPT_SetPriority(SysTick_IRQn, 0);
}

void TIMER_SetSystemClockSpeed(SystemClockSpeed SCS_x){
    SCS_IN_MHz = SCS_x / MS_PER_SECOND / US_PER_MS;     // Divide by 1,000,000 (72MHz -> 72)
    SysTick->LOAD = ((SCS_x / MS_PER_SECOND) - 1) & SysTick_LOAD_RELOAD_Msk;
}

void SysTick_Handler(void){
    SysTickCounter += 1;
}

uint64_t Millis(){
    return SysTickCounter;
}

uint64_t Micros(){
    uint32_t numSysTicks = 1000 - (SysTick->VAL / SCS_IN_MHz);   // Current number of ticks in the value register (SysTick counts down from SysTick->LOAD)
    return (SysTickCounter * US_PER_MS) + numSysTicks;           // Gets the total number of us from startup (MS * 1000) + us 
}

void Delay(uint64_t delayVal){
    uint64_t current = Millis();
    uint64_t wait = delayVal;
    while(Millis() - current < wait){}
}