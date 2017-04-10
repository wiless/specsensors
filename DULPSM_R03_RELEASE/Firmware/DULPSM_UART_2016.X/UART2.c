#include <xc.h>
#include "delay.h"

void UART2_init(void)
{
    U2MODEbits.UARTEN = 0;
	U2MODEbits.WAKE = 1;
    U2BRG = 25;	
    U2MODEbits.UARTEN = 1;	
    U2STAbits.UTXEN = 1;
}

void UART2_xmitc(char UART2_char_input)
{
    while(!IFS1bits.U2TXIF);        
    U2TXREG = UART2_char_input;
    delay(400);
}

void UART2_xmits(char* UART2_string_input)
{
    while(*UART2_string_input != 0)
    {
        UART2_xmitc(*UART2_string_input++);
    }
}

void UART2_xmits_NewLine(char* UART2_string_input)
{
    while(*UART2_string_input != 0)
    {
        UART2_xmitc(*UART2_string_input++);
    }
    UART2_xmitc('\r');
    UART2_xmitc('\n');
}