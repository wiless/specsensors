#ifndef UART2_H
#define	UART2_H

#include <xc.h> 
#include "delay.h"

void UART2_init(void);
void UART2_xmitc(char UART2_char_input);
void UART2_xmits(char* UART2_string_input);
void UART2_xmits_NewLine(char* UART2_string_input);

#endif

