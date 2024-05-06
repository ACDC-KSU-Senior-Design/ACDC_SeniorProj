# ACDC_TIMER.h

All functions below assume that you have included **"ACDC_TIMER.h"**

## Use the Delay_MS function to toggle a LED every second

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

//Green LED => GPIOA, GPIO_PIN_5

int main(){

    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (CALLS TIMER_Init)
    GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);   //Set the Green LED to an output

    while(1){
        Delay_MS(1000);                    // Delays for 1000ms
        GPIO_Toggle(GPIOA, GPIO_PIN_5); //Toggles the Green LED
    }
}
```

## Use the Millis function to toggle a LED every second

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

//Green LED => GPIOA, GPIO_PIN_5

int main(){

    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (CALLS TIMER_Init)
    GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);   //Set the Green LED to an output

    uint32_t previousTime = Millis();   //Grab the number of milliseconds since startup

    while(1){
        if(Millis() - previousTime >= 1000){    // 1,000ms per second
            previousTime = Millis();            // Update this first to keep accurate timing
            GPIO_Toggle(GPIOA, GPIO_PIN_5);     // (else it will drift because of the time it takes to call GPIO_Toggle)
        }
    }

}
```

## Use the Micros function to toggle a LED every second

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

//Green LED => GPIOA, GPIO_PIN_5

int main(){

    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (CALLS TIMER_Init)
    GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);   //Set the Green LED to an output

    uint64_t previousTime = Micros();   //Grab the number of milliseconds since startup

    while(1){
        if(Micros() - previousTime >= 1000000){ // 1,000,000us per second
            previousTime = Micros();            // Update this value first to keep accurate timing
            GPIO_Toggle(GPIOA, GPIO_PIN_5);     // (else it will drift because of the time it takes to call GPIO_Toggle)
        }
    }

}
```

## Create a 1MHz Clock on PA11 using PWM

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

void main(){
    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz

    TIMER_PWM_Init(TIM1_CH4_PA11, PWM_MODE_1, 1000000);                         // Needed to drive the ARINC429 Clock
    TIMER_PWM_SetDuty(TIM1_CH4_PA11, TIMER_PWM_GetPeriod(TIM1_CH4_PA11) / 2);   // Set the Duty cycle to 50%

    while(1){}
}
```

## Fade in and out an LED on PA7 using PWM

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

void main(){
    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz

    TIMER_PWM_Init(TIM3_CH2_PA7, PWM_MODE_1, 100000);   // Create 100KHz Frequency
    uint32_t timPeriod = TIMER_PWM_GetPeriod(TIM3_CH2_PA7);
    int8_t incrementValue = 10;

    while (1)
    {
        int64_t currVal = TIMER_PWM_GetDuty(TIM3_CH2_PA7);                          // Get the current duty value
        if(currVal + incrementValue <= 0 || currVal + incrementValue >= timPeriod)  // If the new value is less than 0 or greator than the period
          incrementValue *= -1;                                                     // Change the incrementation direction
        
        TIMER_PWM_SetDuty(TIM3_CH2_PA7, currVal + incrementValue);                  // Set the new value of the LED
        Delay_MS(10);
    }
}
```

![LED Pulsing on Waveforms](Photos\Pulsing_LED_Waveforms.gif)

![LED Pulsing video](Photos\Pulsing_LED_Video.gif)