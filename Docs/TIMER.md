# ACDC_TIMER.h

All functions below assume that you have included **"ACDC_TIMER.h"**

## Use the Delay function to toggle a LED every second

```C
#include "ACDC_TIMER.h"
#include "ACDC_CLOCK.h"

//Green LED => GPIOA, GPIO_PIN_5

int main(){

    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (CALLS TIMER_Init)
    CLOCK_SetAPB1Prescaler(APB_DIV_2);      //Set the APB1 Prescaler to /2

    GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);   //Set the Green LED to an output

    while(1){
        Delay(1000);                    // Delays for 1000ms
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

    CLOCK_SetSystemClockSpeed(SCS_72MHz);   //Set the SysClock to 72MHz (Calls TIMER_Init)
    CLOCK_SetAPB1Prescaler(APB_DIV_2);      //Set the APB1 Prescaler to /2

    GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);   //Set the Green LED to an output

    uint32_t previousTime = Millis();   //Grab the number of milliseconds since startup

    while(1){
        if(Millis() - previousTime >= 1000){
            GPIO_Toggle(GPIOA, GPIO_PIN_5);
            previousTime = Millis();
        }
    }

}
```
