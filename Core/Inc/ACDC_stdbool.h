/**
 * @file ACDC_stdbool.h
 * @author Devin Marx
 * @brief Header file for the type bool
 * @version 0.1
 * @date 2023-10-10
 * 
 * @copyright Copyright (c) 2023-2024
 * 
 */

#ifndef __ACDC_STDBOOL_H
#define __ACDC_STDBOOL_H

typedef enum{
    true = 1 ,    /**< A Value of True or 1     */
    false = !true /**< A Value that is not True */
}bool;

#endif