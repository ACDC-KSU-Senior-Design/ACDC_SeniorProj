/**
 * @file ACDC_Assert.h
 * @author Devin Marx
 * @brief Header file containing compile time and runtime macros along with the assertion handler.
 * @version 0.1
 * @date 2024-01-21
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef __ACDC_ASSERT_H
#define __ACDC_ASSERT_H

#pragma GCC diagnostic ignored "-Wunused-variable"  //Removes the -Wunsued-variable error when a static assert is thrown (for variable STATIC_ASSERTION_FAILED)

/// @brief Checks the value of COND at compile time and throws a compile-time error if COND is not true.
/// @param COND Condition to be checked
/// @param MSG Message to be shown if the compile time assertion fails
#define STATIC_ASSERT(COND, MSG)                                    \
        do {                                                        \
            char* STATIC_ASSERTION_FAILED[(COND) ? 1 : -1] = {MSG}; \
        } while(0)                                                  \

/// @brief Checks the value of x at runtime and calls the AssertHandler if the condition is not true.
/// @param x Condition to be checked
#define ASSERT(x)                \
        do {                     \
            if((bool)!x){        \
                AssertHandler(); \
            }                    \
        } while (0)              \                    

/// @brief Assertion handler function called when a runtime assertion fails. A software breakpoint is triggered and the onboard LED blinks.
void AssertHandler(void);

#endif