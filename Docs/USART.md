# ACDC_USART.h

All functions below assume that you have included **"ACDC_USART.h"**

## Initilize USART2 and print a message every 500ms

```C
#include "ACDC_CLOCK.h"

// USART2 is connected through the USB port on the NUCLEO-F103RB

int main(void){
    CLOCK_SetSystemClockSpeed(SCS_72Mhz);       // Initilizes the System Clock to 72MHz
    USART_Init(USART2, Serial_115200, true);    // Initilizes USART2 to 115200 baud (uses UART not USART)

    char someData[100] = "ACDC Senior Project!";    // Message to send

    while(1){
        USART_SendString(USART2, someData); // Send the message by polling the registers (Blocking code)
        Delay(500);                         // Wait half a second
    }
}

```

## Initilize USART2 and read a message from the Serial Monitor (Change Line ending to "CRLF")

```C
#include "ACDC_CLOCK.h"

char recieveBuffer[50];    // Buffer that will contain our message (49 bytes of data, 1 byte of '\0')

int main(void){
    CLOCK_SetSystemClockSpeed(SCS_72MHz);       // Initilizes the System Clock to 72MHz
    USART_Init(USART2, Serial_115200, true);    // Initilizes USART2 to 115200 baud (uses UART not USART)

    while(1){

        //Other Code here

        if(USART_HasDataToRecieve(USART2)){
            USART_RecieveString(USART2, recieveBuffer, sizeof(recieveBuffer));  // Retrieve the desired data
            USART_SendString(USART2, recieveBuffer);    // Print the recieved data on the Serial Monitor
        }
    }
}
```
