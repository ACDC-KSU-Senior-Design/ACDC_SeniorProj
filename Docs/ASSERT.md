# ACDC_Assert.h

All functions below assume you have included **"ACDC_Assert.h"**

## Static Assert (Compile Time Assert)

### Static Assert Causing Compile-time Error

```C
#define GREEN_LED_PORT GPIOA
#define GREEN_LED_PIN 0x20

void main(){
    STATIC_ASSERT(GREEN_LED_PIN == GPIO_PIN_8, "Assert that green led pin is GPIO_PIN_5"); // This will cause an error an will not compile
}
```

### Static Assert without Error

```C
#define GREEN_LED_PORT GPIOA
#define GREEN_LED_PIN 0x20

void main(){
    STATIC_ASSERT(GREEN_LED_PIN == GPIO_PIN_5, "Assert that green led pin is GPIO_PIN_5"); // This works and compiles the code normally
}
```

## Assert (Runtime Assert)

### Assert that causes runtime error

```C
void main(){
    int someNum = addTen(1); // Function should take 1 and return 11
    ASSERT(someNum == 11);   // Function actually returns 2 and the assert will be triggered

    /***************************************************************************************
     * If the programm is running in debug mode with cortex-debug then a breakpoint will be
     * triggered on the line containing the assert. If the assert is triggered and is not 
     * running in debug mode, then the onboard led (Green) will flash and code execution
     * will be paused until the program / board is reset.
     ***************************************************************************************/

    /** Assert statements should be inside implementation files for device drivers **/

    /** More Code **/

    while(true){}
}

int addTen(int someNum){
    return someNum + 1;     // Should add 10 not 1 (So assert will fire)
}
```

### Assert that does not cause runtime error

```C
void main(){
    int someNum = addOne(1); // Function should take 1 and return 2
    ASSERT(someNum == 2);    // Function returns 2 so code continues

    /**
     * If the assert is triggered, whether or not it is in debug mode, code execution will
     * continue as it normally would.
     **/

    /** More Code **/

    while(true){}
}

int addOne(int someNum){
    return someNum + 1;
}
```
