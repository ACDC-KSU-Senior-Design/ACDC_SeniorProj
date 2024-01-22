/**
 * @file ACDC_TIMER.h
 * @author Devin Marx
 * @brief Header file containing functions for system timing using a hardware timer.
 * 
 * This file defines functions for initializing a system clock timer, setting its value, 
 * and providing timing-related functionalities such as retrieving number of milliseconds
 * since startup and introducing delays in code execution.
 * 
 * @version 0.1
 * @date 2023-11-7
 * 
 * @copyright Copyright (c) 2023-2024
 */

#ifndef __ACDC_TIMER_H
#define __ACDC_TIMER_H

#include "ACDC_CLOCK.h"
#include "ACDC_stdint.h"

/// @brief (SHOULD NOT BE CALLED BY USER, CALLED IN ACDC_CLOCK.h) Initilizes the SysClock timer and sets the value of SysClock->LOAD
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void TIMER_Init(SystemClockSpeed SCS_x);

/// @brief (SHOULD NOT BE CALLED BY USER, CALLED IN ACDC_CLOCK.h) Sets the value for SysClock->LOAD
/// @param SCS_x System Clock Speed (Ex. SCS_72Mhz, SCS_36Mhz, ...)
void TIMER_SetSystemClockSpeed(SystemClockSpeed SCS_x);

/// @brief Grabs and returns the total number of milliseconds since the Micro turned on
/// @return Number of milliseconds since startup
uint64_t Millis();

/// @brief Pauses code execution for delayVal number of milliseconds
/// @param delayVal Number of milliseconds to delay for
void Delay(uint64_t delayVal);

#endif