#include "ACDC_CLOCK.h"
#include "ACDC_GPIO.h"

#define MAX_MCO_CLK_SPEED 50000000

static SystemClockSpeed currentSCS; /* Current System Clock Speed */

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

void CLOCK_SetMcoOutput(MicroClockOutput MCO_x){

    GPIO_PinDirection(GPIOA, GPIO_PIN_8, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);

    int regVal = RCC->CFGR & ~(RCC_CFGR_MCO_Msk); // Clears the register

    if(currentSCS > MAX_MCO_CLK_SPEED)
        RCC->CFGR = regVal | RCC_CFGR_MCO_PLLCLK_DIV2;  // Sets the MCO output to 1/2 of PLL Clock
    else
        RCC->CFGR = regVal | MCO_x;                     // Sets the Desired MCO Bits
}


void CLOCK_SetADCPrescaler(ADC_Prescaler ADC_DIV_x){
    uint32_t tempReg = RCC->CFGR & ~RCC_CFGR_ADCPRE_Msk;        // Clear the ADC Prescaler bits in RCC->CFGR
    RCC->CFGR = tempReg | (ADC_DIV_x << RCC_CFGR_ADCPRE_Pos);   // Set the ADC_DIV_x bits in RCC->CFGR
}

void CLOCK_SetAPB1Prescaler(APB_Prescaler APB_DIV_x){
    uint32_t tempReg = RCC->CFGR & ~RCC_CFGR_PPRE1_Msk;         // Clear the APB1 Bits in RCC->CFGR
    RCC->CFGR = tempReg | (APB_DIV_x << RCC_CFGR_PPRE1_Pos);    // Set the APB_DIV_x bits in RCC->CFGR

}

void CLOCK_SetAPB2Prescaler(APB_Prescaler APB_DIV_x){
    uint32_t tempReg = RCC->CFGR & ~RCC_CFGR_PPRE2_Msk;         // Clear the APB2 Bits in RCC->CFGR
    RCC->CFGR = tempReg | (APB_DIV_x << RCC_CFGR_PPRE2_Pos);    // Set the APB_DIV_x bits in RCC->CFGR
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

/* DELETE CODE BELOW */
//RCC->CFGR |= RCC_CFGR_PLLMULL9 | RCC_CFGR_ADCPRE_DIV6;  // PLL Mult = 9, ADC to 12Mhz    
//RCC->CFGR |= RCC_CFGR_PPRE1_DIV2 | RCC_CFGR_PPRE2_DIV1; // Set APB1 Peripheral to 36Mhz, Set APB2 Peripheral to 72Mhz
/* DELETE CODE ABOVE*/