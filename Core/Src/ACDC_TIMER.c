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
#include "ACDC_GPIO.h"
#include "ACDC_INTERRUPT.h"
#include "stm32f1xx.h"

#define MS_PER_SECOND 1000  // Number of milliseconds per second
#define US_PER_MS     1000  // Number of microseconds per millisecond

volatile static uint64_t SysTickCounter;
static uint8_t SCS_IN_MHz;                  // Clock frequency in MHz (SCS_72Mhz -> 72, SCS_36Mhz -> 36, Etc.)

#pragma region PRIVATE_FUNCTION_PROTOTYPES
/// @brief Enables the TIMx peripheral clock
/// @param TIMx Timer to initialize clock for (Ex. TIM1, TIM2, ...)
static void TIMER_InitClk(const TIM_TypeDef *TIMx);

/// @brief Sets the PWM mdoe for the specified timer and channel
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @param PWM_MODE_x Desired PWM mode (Ex. PWM_MODE_1 or PWM_MODE_2)
static void TIMER_PWM_SetPwmMode(TIMx_CHx TIMx_CHx_Pxx, PWM_MODE PWM_MODE_x);

/// @brief Enables or disables the output signal for the specified timer and channel.
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @param enable True if you want to enable the output signal, else false to disable
static void TIMER_PWM_SetOutputEnable(TIMx_CHx TIMx_CHx_Pxx, bool enable);

/// @brief Enables or disables preloading the output for the specified timer and channel.
/// @param TIMx_CHx_Pxx Struct containing configuration data for the specific timer and channel
/// @param enable True if you want to enable preloading the output, else false to disable
static void TIMER_PWM_SetPreloadEnable(TIMx_CHx TIMx_CHx_Pxx, bool enable);
#pragma endregion

#pragma region PUBLIC_FUNCTIONS
void TIMER_Init(SystemClockSpeed SCS_x){  //TODO: Should only ever be called once (Need to add an assert)
    SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk;
    TIMER_SetSystemClockSpeed(SCS_x);
    INTERRUPT_SetPriority(SysTick_IRQn, 0);
}

void TIMER_SetSystemClockSpeed(SystemClockSpeed SCS_x){
    SCS_IN_MHz = SCS_x / MS_PER_SECOND / US_PER_MS;     // Divide by 1,000,000 (72MHz -> 72)
    SysTick->LOAD = ((SCS_x / MS_PER_SECOND) - 1) & SysTick_LOAD_RELOAD_Msk;
}

void TIMER_PWM_Init(TIMx_CHx TIMx_CHx_Pxx, PWM_MODE PWM_MODE_x, uint32_t frequency){
    TIMER_InitClk(TIMx_CHx_Pxx.TIMx);                                               // Initialize the clock for the timer
    TIMER_PWM_SetPwmMode(TIMx_CHx_Pxx, PWM_MODE_x);                                 // Set the mode to PWM1 or PWM2
    TIMER_PWM_SetDuty(TIMx_CHx_Pxx, 0);                                             // Set the default duty cycle to 0
    TIMER_PWM_SetOutputEnable(TIMx_CHx_Pxx, true);                                  // Enable the output on the pin 
    TIMER_PWM_SetPreloadEnable(TIMx_CHx_Pxx, true);                                 // Enable the output compare preloader
    WRITE_REG(TIMx_CHx_Pxx.TIMx->PSC, 0);                                           // Set Prescaler to DIV 1 (Can change later if need more functionality {See RM-356})
    WRITE_REG(TIMx_CHx_Pxx.TIMx->ARR, (CLOCK_GetSystemClockSpeed() / frequency)-1); // Set the PWM Period (Subtract 1 because its 0 based)

    if(TIMx_CHx_Pxx.TIMx == TIM1)                           // If it is an advanced timer (1 or 8)
        SET_BIT(TIMx_CHx_Pxx.TIMx->BDTR, TIM_BDTR_MOE);     // Enable the master output 
    
    //GPIO Configuration {See RM-166}
    GPIO_PinDirection(TIMx_CHx_Pxx.GPIOx, TIMx_CHx_Pxx.GPIO_PIN_x, GPIO_MODE_OUTPUT_SPEED_50MHz, GPIO_CNF_OUTPUT_AF_PUSH_PULL);   

    SET_BIT(TIMx_CHx_Pxx.TIMx->CR1, TIM_CR1_CEN);
}

void TIMER_PWM_SetDuty(TIMx_CHx TIMx_CHx_Pxx, uint32_t dutyCycle){
    if(dutyCycle > TIMER_PWM_GetPeriod(TIMx_CHx_Pxx))   // If dutyCycle is bigger than 100% duty cycle
        dutyCycle = TIMER_PWM_GetPeriod(TIMx_CHx_Pxx);  // Set it to the max duty cycle

    uint8_t channel = TIMx_CHx_Pxx.TimerChannel;        // Grab the specific channel for the timer
    if(channel == 1)                                    // Check the channel
        TIMx_CHx_Pxx.TIMx->CCR1 = dutyCycle;            // Set the duty cycle for the specific channel
    else if(channel == 2)
        TIMx_CHx_Pxx.TIMx->CCR2 = dutyCycle;
    else if(channel == 3)
        TIMx_CHx_Pxx.TIMx->CCR3 = dutyCycle;
    else if(channel == 4)
        TIMx_CHx_Pxx.TIMx->CCR4 = dutyCycle;
}

uint32_t TIMER_PWM_GetPeriod(TIMx_CHx TIMx_CHx_Pxx){
    return TIMx_CHx_Pxx.TIMx->ARR;
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

void Delay_MS(uint64_t delayVal){
    uint64_t current = Millis();
    while(Millis() - current < delayVal){}
}

void Delay_US(uint64_t delayVal){
    uint64_t current = Micros();
    while(Micros() - current <= delayVal){}
}
#pragma endregion

#pragma region PRIVATE_FUNCTOINS
static void TIMER_InitClk(const TIM_TypeDef *TIMx){
    // TIM1 is the only timer that is located on APB2   
    // TIM2, TIM3, & TIM4 are located on APB1   {See RM-93, RM-113, RM-117}
    if(TIMx == TIM1)
        SET_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN);
    else if(TIMx == TIM2)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN);
    else if(TIMx == TIM3)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN);
    else if(TIMx == TIM4)
        SET_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN);
}

static void TIMER_PWM_SetPwmMode(TIMx_CHx TIMx_CHx_Pxx, PWM_MODE PWM_MODE_x){
    // CCMR1 and CCMR2 have the same bit structure
    volatile uint16_t *CCMRx = TIMx_CHx_Pxx.TimerChannel < 3 ?  // If it is channel 1 or 2
                                   &TIMx_CHx_Pxx.TIMx->CCMR1 :  // Grab CCMR1
                                   &TIMx_CHx_Pxx.TIMx->CCMR2;   // Else grab CCMR2
    
    if(TIMx_CHx_Pxx.TimerChannel == 1 || TIMx_CHx_Pxx.TimerChannel == 3){   // if it is the first 8 bits on CCMR1 or CCMR2 (CH1 or CH3)
        CLEAR_BIT(*CCMRx, TIM_CCMR1_OC1M_Msk);      // Clear the OCxM bits
        SET_BIT(*CCMRx, PWM_MODE_x);                // Set the desired PWM mode
    } else {                                                                // else its the second 8 bits of CCMR1 or CCMR2 (CH2 or CH4)
        CLEAR_BIT(*CCMRx, TIM_CCMR1_OC1M_Msk << 8); // Clear the upper OCxM bits
        SET_BIT(*CCMRx, PWM_MODE_x << 8);           // Set the upper PWM mode bits
    }
}

static void TIMER_PWM_SetOutputEnable(TIMx_CHx TIMx_CHx_Pxx, bool enable){
    uint16_t bitOffset = (1 << (TIMx_CHx_Pxx.TimerChannel-1) * 4); // using the channel it calculates CCERx

    if(enable)
        SET_BIT(TIMx_CHx_Pxx.TIMx->CCER, bitOffset);
    else 
        CLEAR_BIT(TIMx_CHx_Pxx.TIMx->CCER, bitOffset);
}

static void TIMER_PWM_SetPreloadEnable(TIMx_CHx TIMx_CHx_Pxx, bool enable){
    // CCMR1 and CCMR2 have the same bit structure
    volatile uint16_t *CCMRx = TIMx_CHx_Pxx.TimerChannel < 3 ?  // If it is channel 1 or 2
                                   &TIMx_CHx_Pxx.TIMx->CCMR1 :  // Grab CCMR1
                                   &TIMx_CHx_Pxx.TIMx->CCMR2;   // Else grab CCMR2
    
    if(enable){
        if(TIMx_CHx_Pxx.TimerChannel == 1 || TIMx_CHx_Pxx.TimerChannel == 3)   // if it is the first 8 bits on CCMR1 or CCMR2 (CH1 or CH3)
            SET_BIT(*CCMRx, TIM_CCMR1_OC1PE);
        else                                                                   // else its for the second 8 bits of CCMR1 or CCMR2 (CH2 or CH4)
            SET_BIT(*CCMRx, TIM_CCMR1_OC2PE);        
    } else {
        if(TIMx_CHx_Pxx.TimerChannel == 1 || TIMx_CHx_Pxx.TimerChannel == 3)   
            CLEAR_BIT(*CCMRx, TIM_CCMR1_OC1PE);
        else                                                                  
            CLEAR_BIT(*CCMRx, TIM_CCMR1_OC2PE);  
    }
}
#pragma endregion