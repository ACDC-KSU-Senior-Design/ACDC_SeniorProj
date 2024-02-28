#include "ACDC_LTC1298_ADC.h"
#include "ACDC_SPI.h"
#include "ACDC_GPIO.h"
#include "ACDC_CLOCK.h"

// LTC1298 ADC Config {See LTC1298-12}
#define ADC_TRANSMISSON_START     0b1000    /** Tells the ADC that a transmisson has started */
#define ADC_MUX_MODE_SINGLE_ENDED 0b0100    /** Sets the ADC into SINGLE-ENDED Mode          */
#define ADC_MUX_MODE_DIFFERENTIAL 0b0000    /** Sets the ADC into DIFFERENTIAL Mode          */
#define ADC_MUX_CHANNEL_0         0b0000    /** Enables Recieving data from Channel 0        */
#define ADC_MUX_CHANNEL_1         0b0010    /** Enables Recieving data from Channel 1        */ 
#define ADC_MUX_MSBFIRST          0b0001    /** Sets the Recieved data format to MSB First   */
#define ADC_MUX_LSBFIRST          0b0000    /** Sets the Recieved data format to LSB First   */
#define MAX_CLOCK_SPEED           200000    /** ADC MAX Clock Freq = 200kHz {See LTC1298-14} */


void LTCADC_Init(SPI_TypeDef *SPIx){
    if(SPIx == SPI1)
        SPI_EnableRemap(SPIx, true); // Enable pin remapping on SPI1 so it has 5v tolarant pins
    SPI_Init(SPIx, true);            // Enable SPIx and set it as the master
    SPI_CalculateAndSetBaudDivider(SPIx, MAX_CLOCK_SPEED); //TODO: Need to make a way to ASSERT if the CLOCK Speed is set low enough (BREAK IF NOT)
}

void LTCADC_InitCS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN){
    LTCADC_Init(SPIx);
    GPIO_PinDirection(GPIOx, GPIO_PIN, GPIO_MODE_OUTPUT_SPEED_2MHz, GPIO_CNF_OUTPUT_PUSH_PULL);
}

uint16_t LTCADC_ReadCH0(SPI_TypeDef *SPIx){
    uint16_t dataToTransmit = ADC_TRANSMISSON_START | ADC_MUX_MODE_SINGLE_ENDED | ADC_MUX_CHANNEL_0 | ADC_MUX_MSBFIRST;
    SPI_Transmit(SPIx, dataToTransmit);         // Send the data to star the transmission
    return SPI_TransmitReceive(SPIx, 0) >> 3;   // Data to send does not matter {See LTC1298-11}
}

uint16_t LTCADC_ReadCH1(SPI_TypeDef *SPIx){
    uint16_t dataToTransmit = ADC_TRANSMISSON_START | ADC_MUX_MODE_SINGLE_ENDED | ADC_MUX_CHANNEL_1 | ADC_MUX_MSBFIRST;
    SPI_Transmit(SPIx, dataToTransmit);         // Send the data to start the tranmission
    return SPI_TransmitReceive(SPIx, 0) >> 3;   // Data to send does not matter {See LTC1298-11}
}

uint16_t LTCADC_ReadCH0CS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x){
    GPIO_Clear(GPIOx, GPIO_PIN_x);              // Set the Chip Select Low
    uint16_t adcData = LTCADC_ReadCH0(SPIx);    // Read the value from the ADC
    while(SPIx->SR & SPI_SR_BSY){}              // Wait until SPIx is done
    GPIO_Set(GPIOx, GPIO_PIN_x);                // Set the Chip Select High
    return adcData;
}

uint16_t LTCADC_ReadCH1CS(SPI_TypeDef *SPIx, GPIO_TypeDef *GPIOx, uint16_t GPIO_PIN_x){
    GPIO_Clear(GPIOx, GPIO_PIN_x);              // Set the Chip Select Low
    uint16_t adcData = LTCADC_ReadCH1(SPIx);    // Read the value from the ADC
    while(READ_BIT(SPIx->SR, SPI_SR_BSY)){}     // Wait until SPIx is done
    GPIO_Set(GPIOx, GPIO_PIN_x);                // Set the Chip Select High
    return adcData;
}