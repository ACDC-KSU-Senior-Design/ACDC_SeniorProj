#ifndef __ACDC_GPIO_H
#define __ACDC_GPIO_H

#include "stm32f103xb.h"
#include "ACDC_stdint.h"

#define GPIO_PIN_0                 ((uint16_t)0x0001)  /* Pin 0 selected    */
#define GPIO_PIN_1                 ((uint16_t)0x0002)  /* Pin 1 selected    */
#define GPIO_PIN_2                 ((uint16_t)0x0004)  /* Pin 2 selected    */
#define GPIO_PIN_3                 ((uint16_t)0x0008)  /* Pin 3 selected    */
#define GPIO_PIN_4                 ((uint16_t)0x0010)  /* Pin 4 selected    */
#define GPIO_PIN_5                 ((uint16_t)0x0020)  /* Pin 5 selected    */
#define GPIO_PIN_6                 ((uint16_t)0x0040)  /* Pin 6 selected    */
#define GPIO_PIN_7                 ((uint16_t)0x0080)  /* Pin 7 selected    */
#define GPIO_PIN_8                 ((uint16_t)0x0100)  /* Pin 8 selected    */
#define GPIO_PIN_9                 ((uint16_t)0x0200)  /* Pin 9 selected    */
#define GPIO_PIN_10                ((uint16_t)0x0400)  /* Pin 10 selected   */
#define GPIO_PIN_11                ((uint16_t)0x0800)  /* Pin 11 selected   */
#define GPIO_PIN_12                ((uint16_t)0x1000)  /* Pin 12 selected   */
#define GPIO_PIN_13                ((uint16_t)0x2000)  /* Pin 13 selected   */
#define GPIO_PIN_14                ((uint16_t)0x4000)  /* Pin 14 selected   */
#define GPIO_PIN_15                ((uint16_t)0x8000)  /* Pin 15 selected   */
#define GPIO_PIN_All               ((uint16_t)0xFFFF)  /* All pins selected */

#define GPIO_MODE_INPUT              ((uint16_t)0b00)  /* GPIO Input Mode          */
#define GPIO_MODE_OUTPUT_SPEED_2MHz  ((uint16_t)0b10)  /* GPIO Output Mode @ 2Mhz  */
#define GPIO_MODE_OUTPUT_SPEED_10MHz ((uint16_t)0b01)  /* GPIO Ouptut Mode @ 10Mhz */
#define GPIO_MODE_OUTPUT_SPEED_50MHz ((uint16_t)0b11)  /* GPIO Output Mode @ 50Mhz */

#define GPIO_CNF_OUTPUT_PUSH_PULL       ((uint16_t)0b00)   /* GPIO Output Mode Push-Pull                     */
#define GPIO_CNF_OUTPUT_OPEN_DRAIN      ((uint16_t)0b01)   /* GPIO Output Mode Open-Drain                    */
#define GPIO_CNF_OUTPUT_AF_PUSH_PULL    ((uint16_t)0b10)   /* GPIO Alternate Function Output Mode Push-Pull  */
#define GPIO_CNF_OUTPUT_AF_OPEN_DRAIN   ((uint16_t)0b11)   /* GPIO Alternate Function Output Mode Open Drain */

#define GPIO_CNF_INPUT_ANALOG       ((uint16_t)0b000)  /* GPIO Input Mode Analog   */
#define GPIO_CNF_INPUT_FLOATING     ((uint16_t)0b001)  /* GPIO Input Mode Floating */
#define GPIO_CNF_INPUT_PULLDOWN     ((uint16_t)0b010)  /* GPIO Input Mode Pulldown */
#define GPIO_CNF_INPUT_PULLUP       ((uint16_t)0b110)  /* GPIO Input Mode Pullup   */

/// @brief Enables the GPIOx peripheral clock (Needed to read from a pin)
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...) 
void GPIO_InitClk(GPIO_TypeDef *GPIOx);

/// @brief Sets the Direction of the Pin to Input/Output (MODE) and the sets its Configuration (CNF) [CAN ONLY SET ONE PIN AT A TIME]
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...) 
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @param GPIO_MODE Desired mode like output 2Mhz, output 10Mhz, output 50Mhz or Input
/// @param GPIO_CNF Desired configuration, if output then Push-Pull or Open-Drain, else if input then Analog, Floating, or Pullup/Pulldown
void GPIO_PinDirection(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t GPIO_MODE, uint8_t GPIO_CNF);

/// @brief Sets the pins output to the value passed into GPIO_VALUE.
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...) 
/// @param GPIO_VALUE Value to set pin to. If >= 1 then 1, else 0
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint16_t GPIO_VALUE);

/// @brief Sets the pins output to HIGH or 1
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
void GPIO_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/// @brief Sets the pins output to LOW or 0
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...) 
void GPIO_Clear(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/// @brief Toggles the pins output. From 0 -> 1 or 1 -> 0
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...)
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...) 
void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);

/// @brief Returns the current value of GPIO_PIN on GPIOx
/// @param GPIOx Port of the GPIO (Ex. GPIOA, GPIOB, ...) 
/// @param GPIO_PIN Desired pin on port GPIOx (Ex. GPIO_PIN_0, GPIO_PIN_1, ...)
/// @return >= 1 if Pin is High, else 0
uint8_t GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN);
#endif