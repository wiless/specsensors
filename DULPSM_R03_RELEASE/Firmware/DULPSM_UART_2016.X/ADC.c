#include <xc.h>

void ADC_init(void)
{
    AD1CON1bits.ADON = 0;
    AD1CON1bits.MODE12 = 1;
    AD1CON1bits.SSRC = 7;
    AD1CON1bits.FORM = 0;
    AD1CON1bits.ADSIDL = 1;
    AD1CON2bits.PVCFG = 1;
    AD1CON2bits.NVCFG0 = 0;
    AD1CON2bits.BUFREGEN = 0;
    AD1CON2bits.SMPI = 0;
    AD1CON3bits.ADRC = 0;  
    AD1CON3bits.SAMC = 7;
    AD1CON3bits.ADCS = 29;
}

unsigned int ADC_Read(unsigned int channel)
{
    unsigned long int i;
    unsigned long int ADCResult_long;

    AD1CHSbits.CH0SA = channel;
    AD1CHSbits.CH0NA = 0;
    AD1CON1bits.ADON = 1; 
    AD1CON1bits.SAMP = 1; 
    while (!AD1CON1bits.DONE){}; 
    ADCResult_long = 0;
    i = 0;
    while(i < 4097)
    {
        AD1CON1bits.SAMP = 1; 
        while (!AD1CON1bits.DONE){}; 
        ADCResult_long = ADCResult_long + ADC1BUF0;
        ClrWdt();
        i = i + 1;
    }
    AD1CON1bits.ADON = 0;
    ADCResult_long = (unsigned long int)ADCResult_long >> 8;
    ADCResult_long = (unsigned long int)ADCResult_long & 0xFFFF;
    
    return (unsigned int)ADCResult_long;
}

unsigned int ADC_Read_Oversample(void)
{
    unsigned long int ADC_long;
    unsigned int i;
    
    ADC_long = 0;
    i = 0;
    while(i < 64)
    {
        ADC_long = ADC_long + ADC_Read(16);
        
        ClrWdt();
        i++;
    }
    ADC_long = ADC_long >> 6;
    
    return (unsigned int)ADC_long;
}
