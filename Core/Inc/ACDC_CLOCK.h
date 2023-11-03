#ifndef __ACDC_CLOCK_H
#define __ACDC_CLOCK_H

#include "stm32f103xb.h"

typedef enum{
    SCS_1MHz  = 1000000,
    SCS_2MHz  = 2000000,
    SCS_3MHz  = 3000000,
    SCS_4MHz  = 4000000,
    SCS_5MHz  = 5000000,
    SCS_6MHz  = 6000000,
    SCS_7MHz  = 7000000,
    SCS_8MHz  = 8000000,
    SCS_9MHz  = 9000000,
    SCS_10MHz = 10000000,
    SCS_11MHz = 11000000,
    SCS_12MHz = 12000000,
    SCS_13MHz = 13000000,
    SCS_14MHz = 14000000,
    SCS_15MHz = 15000000,
    SCS_16MHz = 16000000,
    SCS_18MHz = 18000000,
    SCS_20MHz = 20000000,
    SCS_22MHz = 22000000,
    SCS_24MHz = 24000000,   
    SCS_26MHz = 26000000,
    SCS_28MHz = 28000000,
    SCS_30MHz = 30000000,
    SCS_32MHz = 32000000,   
    SCS_36MHz = 36000000,
    SCS_40MHz = 40000000,   
    SCS_44MHz = 44000000,
    SCS_48MHz = 48000000,   
    SCS_52MHz = 52000000,
    SCS_56MHz = 56000000,   
    SCS_60MHz = 60000000,
    SCS_64MHz = 64000000,   
    SCS_72MHz = 72000000    
}SystemClockSpeed;

void CLOCK_SetSystemClockSpeed(SystemClockSpeed SCS_x);

SystemClockSpeed CLOCK_GetSystemClockSpeed(void);

#endif