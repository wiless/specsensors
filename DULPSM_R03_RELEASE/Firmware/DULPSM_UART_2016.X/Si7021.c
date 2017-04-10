#include <xc.h>
#include "i2c2.h"

unsigned int Si7021_Read_T(void)
{
    unsigned int T;

    I2C2_start();
    while(I2C2_tx(0b10000000));
    while(I2C2_tx(0xE3));
    I2C2_stop();I2C2_start();
    while(I2C2_tx(0b10000001));
    T = I2C2_rx(1);
    T = T << 8;
    T |= I2C2_rx(0);
    I2C2_stop();
    T = T & 0xFFFF;
    
    return T;
}

double Si7021_Calculate_T(unsigned int T, int Default_1000x_Temperature_Offset)
{
    double T_float;

    T_float = ((float)T * 175.72 / 65536) - 46.85;
    T_float = T_float + ((double)Default_1000x_Temperature_Offset / 1000);

    return T_float;
}

unsigned int Si7021_Read_RH(void)
{
    unsigned int RH;
    
    I2C2_start();
    while(I2C2_tx(0b10000000));
    while(I2C2_tx(0xE5));
    I2C2_stop();I2C2_start();
    while(I2C2_tx(0b10000001));
    RH = I2C2_rx(1);
    RH = RH << 8;
    RH |= I2C2_rx(0);
    I2C2_stop();
    RH = RH & 0xFFFF;

    return RH;
}

double Si7021_Calculate_RH(unsigned int RH)
{
    double RH_float;

    RH_float = ((float)RH * 125 / 65536) - 6;
    if(RH_float > 100)
    {
        RH_float = 100;
    }
    else if(RH_float < 0)
    {
        RH_float = 0;
    }
    else;

    return RH_float;
}
