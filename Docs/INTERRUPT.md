# ACDC_INTERRUPT.h
All functions below assume that you have included **"ACDC_INTERRUPT.h"**

## Toggle the DevBoards LED using External Interrupts
```C
#include "ACDC_GPIO.h"

//Green LED   => GPIOA, PIN 5
//Blue Button => GPIOC, PIN 13

int main(){
    //Set Green Led to ouput, Blue Button to Input W/ Pullup resistor, and enable External Interrupts
    GPIO_PinDirection(GPIOA, GPIO_PIN_5 , GPIO_MODE_OUTPUT_SPEED_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);
    GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT            , GPIO_CNF_INPUT_PULLUP    );
    GPIO_INT_SetToInterrupt(GPIOC, GPIO_PIN_13, RISING_EDGE);

    while(1){}
}

// Pins 10-15 Interrupt Vector (Found in startup.stm23f103xb.s)
void EXTI15_10_IRQHandler(void)
{
    if (EXTI->PR & EXTI_PR_PR13) // If Pin13 interrupt has been triggered
    {
        GPIO_Toggle(GPIOA, GPIO_PIN_5); // Toggle the LED
        EXTI->PR = EXTI_PR_PR13;        // Clear the pending interrupt
    }
}
```