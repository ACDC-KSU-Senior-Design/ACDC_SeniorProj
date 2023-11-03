#include "ACDC_CLOCK.h"

static SystemClockSpeed currentSCS;

void CLOCK_SetSystemClockSpeed(SystemClockSpeed SCS_x){
    RCC->CR |= RCC_CR_HSEON;              //Enable High Speed External oscillator
    while(!(RCC->CR & RCC_CR_HSERDY)){}   //Wait until the HSE oscillator is ready
    //Calculate and set System Clocks

    uint32_t configReg = RCC->CFGR;
    configReg &= ~(RCC_CFGR_PLLMULL); //Clear Config to be written

    switch (SCS_x){
        case SCS_16MHz: //PLLMULL2
        case SCS_24MHz: //PLLMULL3
        case SCS_32MHz: //PLLMULL4
        case SCS_40MHz: //PLLMULL5
        case SCS_48MHz: //PLLMULL6
        case SCS_56MHz: //PLLMULL7
        case SCS_64MHz: //PLLMULL8
        case SCS_72MHz: //PLLMULL9
        //RCC_CFGR_PLLMULL3
            uint32_t ScsToPllMul = SCS_x / 8000000 - 2; //Converts SCS to the PLL Multiplier
            configReg |= ScsToPllMul << RCC_CFGR_PLLMULL_Pos;
            /* code */
            break;
        
        default:
            break;
    }

    //disable pll
    RCC->CFGR = configReg;
    //enable pll

    currentSCS = SCS_x; // Set currentSCS to SCS_x when clocks are done configuring
}

SystemClockSpeed CLOCK_GetSystemClockSpeed(void){
    return currentSCS;
}