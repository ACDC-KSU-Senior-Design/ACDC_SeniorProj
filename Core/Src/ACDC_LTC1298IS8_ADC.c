#include "ACDC_LTC1298IS8_ADC.h"
#include "ACDC_SPI.h"
#include "ACDC_CLOCK.h"

// LTC1298 ADC Config {See LTC1298-12}
#define ADC_TRANSMISSON_START     0b1000    /** Tells the ADC that a transmisson has started */
#define ADC_MUX_MODE_SINGLE_ENDED 0b0100    /** Sets the ADC into SINGLE-ENDED Mode          */
#define ADC_MUX_MODE_DIFFERENTIAL 0b0000    /** Sets the ADC into DIFFERENTIAL Mode          */
#define ADC_MUX_CHANNEL_0         0b0000    /** Enables Recieving data from Channel 0        */
#define ADC_MUX_CHANNEL_1         0b0010    /** Enables Recieving data from Channel 1        */ 
#define ADC_MUX_MSBFIRST          0b0001    /** Sets the Recieved data format to MSB First   */
#define ADC_MUX_LSBFIRST          0b0000    /** Sets the Recieved data format to LSB First   */


void LTCADC_Init(SPI_TypeDef *SPIx){
    if(SPIx == SPI1)
        SPI_EnableRemap(SPIx, true); // Enable pin remapping on SPI1 so it has 5v tolarant pins
    SPI_Init(SPIx, true);            // Enable SPIx and set it as the master
    
    //TODO: Create a way to automatically select the fastest SPI baud rate for the ADC (200kHz)
    SPI_SetBaudDivider(SPIx, SPI_BAUD_DIV_256); // ADC MAX Clock Freq = 200kHz {See LTC1298-14} 
}

uint16_t LTCADC_ReadCH0(SPI_TypeDef *SPIx){
    uint16_t dataToTransmit = ADC_TRANSMISSON_START | ADC_MUX_MODE_SINGLE_ENDED | ADC_MUX_CHANNEL_0 | ADC_MUX_MSBFIRST;
    return SPI_TransmitReceive(SPIx, dataToTransmit) >> 4;
}

uint16_t LTCADC_ReadCH1(SPI_TypeDef *SPIx){
    uint16_t dataToTransmit = ADC_TRANSMISSON_START | ADC_MUX_MODE_SINGLE_ENDED | ADC_MUX_CHANNEL_1 | ADC_MUX_MSBFIRST;
    return SPI_TransmitReceive(SPIx, dataToTransmit) >> 4;
}