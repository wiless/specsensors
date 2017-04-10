#ifndef ADC_H
#define	ADC_H

#include <xc.h> 

void ADC_init(void);
unsigned int ADC_Read(unsigned int channel);
unsigned int ADC_Read_Oversample(void);

#endif

