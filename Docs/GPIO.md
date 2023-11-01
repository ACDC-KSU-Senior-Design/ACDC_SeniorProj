# GPIO.h
All functions below assume that you have included **"ACDC_GPIO.h"**

## Set Onboard LED Output
```C
#define LD2_Pin         GPIO_PIN_5
#define LD2_GPIO_Port   GPIOA

void SetNucleoLedToOutput(void){
    GPIO_PinDirection(LD2_GPIO_Port, GPIO_PIN_5, GPIO_MODE_OUTPUT_10MHz , GPIO_CNF_OUTPUT_PUSH_PULL);
}
```

## Set Onboard Button to Input
```C
#define B1_Pin          GPIO_PIN_13
#define B1_GPIO_Port    GPIOC

void SetNucleoButtonToInput(void){
    GPIO_PinDirection(B1_GPIO_Port, B1_Pin, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULLUP);
    GPIO_InitClk(B1_GPIO_Port);
}
```

## Change Onboard LED with onboard Button
```C
#define LD2_Pin         GPIO_PIN_5
#define LD2_GPIO_Port   GPIOA
#define B1_Pin          GPIO_PIN_13
#define B1_GPIO_Port    GPIOC

void SetLedtoButton(void){
    GPIO_PinDirection(LD2_GPIO_Port, GPIO_PIN_5, GPIO_MODE_OUTPUT_10MHz , GPIO_CNF_OUTPUT_PUSH_PULL);
    GPIO_PinDirection(B1_GPIO_Port, B1_Pin, GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULLUP);
    GPIO_InitClk(B1_GPIO_Port);

    uint8_t value = GPIO_Read(B1_GPIO_Port, B1_Pin);
    GPIO_Write(LD2_GPIO_Port, LD2_Pin, value);
}
```

## Set Random GPIO and Pin to Input Floating
```C
void SetGPIOToInputFloating(void){
    GPIO_PinDirection(GPIOA, GPIO_PIN_0, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOATING);
    GPIO_InitClk(GPIOA);
}
```

## Using Set and Clear
```C
void setOutputLed(bool newLedState){
    if(newLedState){
        GPIO_Set(GPIOB, GPIO_PIN_13);
    } else {
        GPIO_Clear(GPIOB, GPIO_PIN_13);
    }
}
```