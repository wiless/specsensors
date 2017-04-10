#include <xc.h>
#include "i2c2.h"

void LMP91000_Unlock(void)
{
    I2C2_start();
    while(I2C2_tx(0b10010000));
    while(I2C2_tx(0x01));
    while(I2C2_tx(0b00000000));
    I2C2_stop();
}

void LMP91000_Write_0x10(unsigned int input)
{
    I2C2_start();
    while(I2C2_tx(0b10010000));
    while(I2C2_tx(0x10));
    while(I2C2_tx(input));
    I2C2_stop();
}

void LMP91000_Write_0x11(unsigned int input)
{
    I2C2_start();
    while(I2C2_tx(0b10010000));
    while(I2C2_tx(0x11));
    while(I2C2_tx(input));
    I2C2_stop();
}

void LMP91000_Write_0x12(unsigned int input)
{
    I2C2_start();
    while(I2C2_tx(0b10010000));
    while(I2C2_tx(0x12));
    while(I2C2_tx(0b00000011));
    I2C2_stop();
}

unsigned int LMP91000_Read(unsigned int input)
{
    unsigned int LMP91000;
    
    I2C2_start();
    while(I2C2_tx(144));
    while(I2C2_tx(input));
    I2C2_stop();
    I2C2_start();
    while(I2C2_tx(145));
    LMP91000 = I2C2_rx(0);
    I2C2_stop();
    
    return LMP91000;
}
