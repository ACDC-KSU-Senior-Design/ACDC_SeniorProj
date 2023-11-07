#include "ACDC_TIMER.h"

volatile static uint64_t SysTickCounter;

void TIMER_Init(void){
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
    SysTick->LOAD = (72000 - 1) & SysTick_LOAD_RELOAD_Msk; //Need to Calcualate A Value
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