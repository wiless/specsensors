#ifndef Si7021_H
#define	Si7021_H

#include <xc.h> 
#include "i2c2.h"

unsigned int Si7021_Read_T(void);
double Si7021_Calculate_T(unsigned int T, int Default_1000x_Temperature_Offset);
unsigned int Si7021_Read_RH(void);
double Si7021_Calculate_RH(unsigned int RH);

#endif

