#ifndef __ACDC_TIMER_H
#define __ACDC_TIMER_H
#include "stm32f103xb.h"

void TIMER_Init(void);

/// @brief Grabs and returns the total number of milliseconds since the Micro turned on
/// @return Number of milliseconds since startup
uint64_t Millis();

void Delay(uint64_t delayVal);

#endif