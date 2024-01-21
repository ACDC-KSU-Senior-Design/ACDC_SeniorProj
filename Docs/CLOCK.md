# ACDC_CLOCK.h
All functions below assume that you have included **"ACDC_CLOCK.h"**

# Set the SysClock to 72Mhz (Max Speed)
```C
#include "ACDC_CLOCK.h"

int main(){

    CLOCK_SetSystemClockSpeed(SCS_72Mhz);   // Highest frequency SysClock can be is 72MHz
    CLOCK_SetAPB1Prescaler(APB_DIV_2);      // Highest frequency APB1 can be is 36MHz
    CLOCK_SetAPB2Prescaler(APB_DIV_1);      // Highest Frequency APB2 can be is 72MHz

    while(1){}
}
```

# Set MCO output to 36Mhz (Enables Clock output on PA8)
```C
#include "ACDC_CLOCK.h"

int main(){

    CLOCK_SetSystemClockSpeed(SCS_36MHz);   // SysClock Freq = 36MHz
    CLOCK_SetAPB1Prescaler(APB_DIV_1);      // APB1 Freq = 36MHz
    CLOCK_SetAPB2Prescaler(APB_DIV_1);      // APB2 Freq = 36MHz
    CLOCK_SetMcoOutput(MCO_SYSCLK);         // Set MCO output to SysClock

    while(1){}
}
```