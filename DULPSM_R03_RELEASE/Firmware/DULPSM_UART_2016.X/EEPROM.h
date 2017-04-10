#ifndef EEPROM_H
#define	EEPROM_H

#include <xc.h> 
#include "delay.h"

void writeNVM(int addr, int newData);
int readNVM (int addr);

#endif

