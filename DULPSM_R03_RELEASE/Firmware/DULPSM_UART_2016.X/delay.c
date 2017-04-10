#include <xc.h>

void delay(unsigned long int count)
{
    unsigned long int dly;
    for (dly = 0; dly < count; dly++)
    {
        Nop();
        ClrWdt();
    }
}
