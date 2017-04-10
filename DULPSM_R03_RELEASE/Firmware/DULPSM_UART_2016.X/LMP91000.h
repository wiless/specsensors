#ifndef LMP91000_H
#define	LMP91000_H

#include <xc.h> 
#include "i2c2.h"

void LMP91000_Unlock(void);
void LMP91000_Write_0x10(unsigned int input);
void LMP91000_Write_0x11(unsigned int input);
void LMP91000_Write_0x12(unsigned int input);
unsigned int LMP91000_Read(unsigned int input);

#endif

