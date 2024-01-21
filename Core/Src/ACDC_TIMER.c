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
#include "stm32f1xx.h"

#define MS_PER_SECOND 1000

volatile static uint64_t SysTickCounter;

void TIMER_Init(SystemClockSpeed SCS_x){  //TODO: Should only ever be called once (Need to add an assert)
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
    TIMER_SetSystemClockSpeed(SCS_x);
}

void TIMER_SetSystemClockSpeed(SystemClockSpeed SCS_x){
    SysTick->LOAD = ((SCS_x / MS_PER_SECOND) - 1) & SysTick_LOAD_RELOAD_Msk;
}

void SysTick_Handler(void){
    SysTickCounter += 1;
}

uint64_t Millis(){
    return SysTickCounter;
}

void Delay(uint64_t delayVal){
    uint64_t current = Millis();
    uint64_t wait = delayVal;
    while(Millis() - current < wait){}
}