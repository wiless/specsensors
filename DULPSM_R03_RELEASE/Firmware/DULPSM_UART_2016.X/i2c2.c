#include <xc.h>

#define SCL2     TRISBbits.TRISB3 
#define SDA2     TRISBbits.TRISB2 
#define SCL2_IN  PORTBbits.RB3    
#define SDA2_IN  PORTBbits.RB2    
#define SCL2_OUT LATBbits.LATB3    
#define SDA2_OUT LATBbits.LATB2    

void I2C2_dly(void)
{
    int dly;
    for (dly = 0; dly < 10; dly++){}
}

void I2C2_init(void)
{
    SDA2 = 1;
    SCL2 = 1;
    SCL2_OUT = 0;
    SDA2_OUT = 0;
}

void I2C2_start(void)
{
    SDA2 = 1;    
    I2C2_dly();
    SCL2 = 1;    
    I2C2_dly();
    SDA2 = 0;    
    I2C2_dly();
    SCL2 = 0;     
    I2C2_dly();
}

void I2C2_stop(void)
{
    I2C2_dly();
    SDA2 = 0;             
    I2C2_dly();
    SCL2 = 1;
    I2C2_dly();
    SDA2 = 1;
    I2C2_dly();
}

char I2C2_tx(unsigned char d)
{
    char x;
    static char b;

    SCL2 = 0;
    for(x=8; x; x--)
    {
        if(d&0x80)
        SDA2 = 1;
        else
        SDA2 = 0;
        I2C2_dly();
        SCL2 = 1;
        d <<= 1;
        I2C2_dly();
        SCL2 = 0;
    }
    SDA2 = 1;
    I2C2_dly();
    SCL2 = 1;
    I2C2_dly();
    b = SDA2_IN;          
    SCL2 = 0;
    return b;
}

unsigned char I2C2_rx(char ack)
{
    char x, d=0;
    SDA2 = 1;    
    SCL2 = 0;            
    for(x=0; x<8; x++)
    {
        I2C2_dly();
        d <<= 1;
        do
        {
            SCL2 = 1;    
        }while(SCL2_IN==0);    
        I2C2_dly();
        if(SDA2_IN) d |= 1;
        SCL2 = 0;    
    }
    if(ack) SDA2 = 0;    
    else SDA2 = 1;        
    I2C2_dly();    
    SCL2 = 1;            
    I2C2_dly();        
    SCL2 = 0;            
    SDA2 = 1;            
    return d;
}

void I2C2_reset(void)
{
    char i;

    SDA2 = 1;  
    for(i=0; i<9; i++)
    {
        SCL2 = 1;  
        I2C2_dly();
        SCL2 = 0;  
        I2C2_dly();
        if(SDA2_IN) break;     
    }
    I2C2_stop();     
}
