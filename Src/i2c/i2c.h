#ifndef i2cH
#define i2cH
#include <util/delay.h>
#include <stddef.h>
#include <avr/io.h>


void i2c_init();
void i2c_start();
void i2c_stop();
void i2c_write(uint8_t data);


#endif