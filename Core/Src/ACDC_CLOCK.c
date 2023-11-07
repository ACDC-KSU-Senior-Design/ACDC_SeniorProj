#include "ACDC_CLOCK.h"

static SystemClockSpeed currentSCS;

/// @brief Disables the HSI and enables the PLL from the SysClock Mux
static void DisableHSI_EnablePLL();

/// @brief Enables the HSI and disables the PLL from the SysClock Mux
static void EnableHSI_DisablePLL();

void CLOCK_SetSystemClockSpeed(SystemClockSpeed SCS_x){

    RCC->CFGR |= RCC_CFGR_PLLSRC;   // Use HSE as PLL Source,
    EnableHSI_DisablePLL();         // Disables the PLL

    // Clears HSE DIV into PLL Source Mux, Clear PLL Mull 
    RCC->CFGR &= ~(RCC_CFGR_PLLXTPRE_Msk | RCC_CFGR_PLLMULL | RCC_CFGR_HPRE_Msk | RCC_CFGR_PPRE1_Msk);

    switch (SCS_x){
        case SCS_16MHz: //PLLMULL2
        case SCS_24MHz: //PLLMULL3
        case SCS_32MHz: //PLLMULL4
        case SCS_40MHz: //PLLMULL5
        case SCS_48MHz: //PLLMULL6
        case SCS_56MHz: //PLLMULL7
        case SCS_64MHz: //PLLMULL8
        case SCS_72MHz: //PLLMULL9
            RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                            // AHB Prescaler DIV1
            RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;                         // HSE No DIV into PLL Src Mux
            RCC->CFGR |= (SCS_x / 8000000 - 2) << RCC_CFGR_PLLMULL_Pos; // Converts SCS into the PLL Multiplier
        break;

        case SCS_8MHz:  //PLLMULL2
        case SCS_12MHz: //PLLMULL3
        case SCS_20MHz: //PLLMULL5
        case SCS_28MHz: //PLLMULL7
        case SCS_36MHz: //PLLMULL9
        case SCS_44MHz: //PLLMULL11
        case SCS_52MHz: //PLLMULL13
        case SCS_60MHz: //PLLMULL15
            RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                            // AHB Prescaler DIV1
            RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;                    // HSE DIV2 into PLL Src Mux
            RCC->CFGR |= (SCS_x / 4000000 - 2) << RCC_CFGR_PLLMULL_Pos; // Converts SCS into the PLL Multiplier 
        break;

        case SCS_4MHz:  //PLLMULL2
        case SCS_6MHz:  //PLLMULL3
        case SCS_10MHz: //PLLMULL5
        case SCS_14MHz: //PLLMULL7
        case SCS_18MHz: //PLLMULL9
        case SCS_22MHz: //PLLMULL11
        case SCS_26MHz: //PLLMULL13
        case SCS_30MHz: //PLLMULL15
            RCC->CFGR |= RCC_CFGR_HPRE_DIV2;                            // AHB Prescaler DIV2
            RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;                    // HSE DIV2 into PLL Src Mux
            RCC->CFGR |= (SCS_x / 2000000 - 2) << RCC_CFGR_PLLMULL_Pos; // Converts SCS into the PLL Multiplier
        break;

        case SCS_2MHz:  //PLLMULL2
        case SCS_3MHz:  //PLLMULL3
        case SCS_5MHz:  //PLLMULL5
        case SCS_7MHz:  //PLLMULL7
        case SCS_9MHz:  //PLLMULL9
        case SCS_11MHz: //PLLMULL11
        case SCS_13MHz: //PLLMULL13
        case SCS_15MHz: //PLLMULL15
            RCC->CFGR |= RCC_CFGR_HPRE_DIV4;                            // AHB Prescaler DIV4
            RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;                    // HSE DIV2 into PLL Src Mux
            RCC->CFGR |= (SCS_x / 1000000 - 2) << RCC_CFGR_PLLMULL_Pos; // Converts SCS into the PLL Multiplier

        case SCS_1MHz:  //PLLMULL2
            RCC->CFGR |= RCC_CFGR_HPRE_DIV8;                            // AHB Prescaler DIV8
            RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;                    // HSE DIV2 into PLL Src Mux
            RCC->CFGR |= (SCS_x / 500000  - 2) << RCC_CFGR_PLLMULL_Pos; // Converts SCS into the PLL Multiplier
        break;

        default:
            break;
    }

    RCC->CFGR |= SCS_x > SCS_36MHz ? RCC_CFGR_PPRE1_DIV2 : RCC_CFGR_PPRE1_DIV1; // Sets the APB1 Prescaler based on SCS

    DisableHSI_EnablePLL(); //Enables the PLL
    currentSCS = SCS_x; // Set currentSCS to SCS_x when clocks are done configuring
}

SystemClockSpeed CLOCK_GetSystemClockSpeed(void){
    return currentSCS;
}

static void DisableHSI_EnableHSE(){
    RCC->CR |= RCC_CR_HSEON;            // Turn on the HSE Clock
    while(!(RCC->CR & RCC_CR_HSERDY)){} // Wait until HSE oscillator is ready

    RCC->CFGR &= ~RCC_CFGR_SW_Msk;      // Clear the SW bits
    RCC->CFGR |= RCC_CFGR_SW_PLL;       // Set PLL as SysClock

    RCC->CR &= ~(RCC_CR_HSION);         // Turn off the HSI Clock
}

static void EnableHSI_DisablePLL(){
    RCC->CR |= RCC_CR_HSION;            // Turn on the HSI Clock
    while(!(RCC->CR & RCC_CR_HSIRDY)){} // Wait until HSI oscillator is ready

    RCC->CFGR &= ~RCC_CFGR_SW_Msk;      // Clean the SW bits
    RCC->CFGR |= RCC_CFGR_SW_HSI;       // Set HSI as SysClock

    RCC->CR &= ~(RCC_CR_HSEON);         // Turn off the HSE Clock
}