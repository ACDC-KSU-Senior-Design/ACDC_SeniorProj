# ACDC_GPIO.h
All functions below assume that you have included **"ACDC_GPIO.h"**

## Set Onboard LED Output
```C
//Green LED   => GPIOA, GPIO_PIN_5
GPIO_PinDirection(GPIOA, GPIO_PIN_5, GPIO_MODE_OUTPUT_10MHz , GPIO_CNF_OUTPUT_PUSH_PULL);
```

## Set Onboard Button to Input W/ Pullup Resistor
```C
//Blue Button => GPIOC, GPIO_PIN_13
GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULLUP);
```

## Set Random GPIO to Input Floating
```C
GPIO_PinDirection(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
```

## Change Onboard LED with onboard Button
```C
//Green LED   => GPIOA, GPIO_PIN_5
//Blue Button => GPIOC, GPIO_PIN_13

int main(){
    //Set Green LED to ouput, Blue Button to Input W/ Pullup Resistor
    GPIO_PinDirection(GPIOA, GPIO_PIN_5 , GPIO_MODE_OUTPUT_10MHz , GPIO_CNF_OUTPUT_PUSH_PULL);
    GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT        , GPIO_CNF_INPUT_PULLUP    );

    while(1){
        uint8_t value = GPIO_Read(GPIOC, GPIO_PIN_13); //Read Blue Button State
        GPIO_Write(GPIOA, GPIO_PIN_5, value);          //Write Blue Button state to Green LED
    }
}
```


## Using Set and Clear
```C
//Green LED   => GPIOA, GPIO_PIN_5
//Blue Button => GPIOC, GPIO_PIN_13

int main(){
    //Set Green LED to ouput, Blue Button to Input W/ Pullup Resistor
    GPIO_PinDirection(GPIOA, GPIO_PIN_5 , GPIO_MODE_OUTPUT_10MHz , GPIO_CNF_OUTPUT_PUSH_PULL);
    GPIO_PinDirection(GPIOC, GPIO_PIN_13, GPIO_MODE_INPUT        , GPIO_CNF_INPUT_PULLUP    );

    while(1){
        uint8_t value = GPIO_Read(GPIOC, GPIO_PIN_13);  //Read Blue Button State
        if(value){
            GPIO_Set(GPIOA, GPIO_PIN_5);    //Set Green LED to 1
        } else {
            GPIO_Clear(GPIOA, GPIO_PIN_5);  //Clear Green LED to 0
        }
    }
}
```