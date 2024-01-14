#include "ACDC_GPIO.h"

#define GPIO_CNF_INPUT_PULLUP_PULLDOWN 0b10u
#define GPIO_MODE_CNF 0b1111
#define GPIO_MODE_OFFSET 0
#define GPIO_CNF_OFFSET 2

void GPIO_InitClk(const GPIO_TypeDef *GPIOx){
    if(GPIOx == GPIOA)
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN; 
    else if(GPIOx == GPIOB)
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    else if(GPIOx == GPIOC)
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    else if(GPIOx == GPIOD)
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    else if(GPIOx == GPIOE)
        RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;
}

void GPIO_PinDirection(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint8_t GPIO_MODE, uint8_t GPIO_CNF){
    GPIO_InitClk(GPIOx);    //Needed in order to use Output / Input

    //Use CRL if it is the first 7 pins else use the CRH 
    volatile uint32_t *REG = (GPIO_PIN <= GPIO_PIN_7) ? &GPIOx->CRL : &GPIOx->CRH;
    uint8_t PIN = GPIO_GetPinNumber(GPIO_PIN);
    if(PIN >= 8)
        PIN -= 8;   // shifts the pin down to accomadate for CRH & CRL

    if(GPIO_MODE == GPIO_MODE_INPUT && (GPIO_CNF & 0b11) == GPIO_CNF_INPUT_PULLUP_PULLDOWN) //If Input & Pullup/Pulldown
        GPIO_Write(GPIOx, GPIO_PIN, (GPIO_CNF & 0b100) >> 2);   
        //ISet GPIOx->ODR to the desired value (Pullup/PullDown)

    //The magic number 4 is the combined size of the MODE and CNF bits (PAGE 172/1136)
    *REG &= ~(GPIO_MODE_CNF << (PIN*4));                    //Clear both the MODE and CNF
    *REG |= (GPIO_MODE << ((PIN*4) + GPIO_MODE_OFFSET));    //Sets the GPIO_MODE (Input / Output)
    *REG |= (GPIO_CNF  << ((PIN*4) + GPIO_CNF_OFFSET ));    //Sets the GPIO_CNF (PP, OD, Analog, PU/PD)
}

void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN, uint16_t GPIO_VALUE){
    if(GPIO_VALUE >= 1)
        GPIO_Set(GPIOx, GPIO_PIN);
    else
        GPIO_Clear(GPIOx, GPIO_PIN);
}

void GPIO_Set(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    GPIOx->ODR |= GPIO_PIN;
}

void GPIO_Clear(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
     GPIOx->ODR &= ~(GPIO_PIN);
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    GPIOx->ODR ^= GPIO_PIN;
}

uint8_t GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    uint8_t PIN = GPIO_GetPinNumber(GPIO_PIN);
    return (GPIOx->IDR & GPIO_PIN) >> PIN;
}

uint8_t GPIO_GetPinNumber(uint16_t GPIO_PIN){
    if(GPIO_PIN == 1)
        return 0u;
    uint8_t pin = 0;
    while(GPIO_PIN >> (1 * ++pin) != 1){}
    return pin;
}