#ifndef i2c_H
#define	i2c_H

#include <xc.h>   

void I2C2_dly(void);
void I2C2_init(void);
void I2C2_start(void);
void I2C2_stop(void);
char I2C2_tx(unsigned char d);
unsigned char I2C2_rx(char ack);
void I2C2_reset(void);

#endif	

