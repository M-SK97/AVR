#ifndef I2C
#define I2C

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/twi.h>

void initI2C(void);
void i2cWaitForComplete(void);
void i2cStart(void);
void i2cStop(void);
uint8_t i2cReadAck(void);
uint8_t i2cReadNoAck(void);

void initI2C(void) {
	TWBR = 32; /* set bit rate, see p. 242 */
	/* 8MHz / (16+2*TWBR*1) ~= 100kHz */
	TWCR |= (1 << TWEN); /* enable */
}

void i2cWaitForComplete(void)
{
	while (!(TWCR & (1<<TWINT))); // loop_until_bit_is_set(TWCR, TWINT);
}

void i2cStart(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	i2cWaitForComplete();
}

void i2cStop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
}

uint8_t i2cReadAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	i2cWaitForComplete();
	return (TWDR);
}

uint8_t i2cReadNoAck(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	i2cWaitForComplete();
	return (TWDR);
}

void i2cSend(uint8_t data)
{
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN); /* init and enable */
	i2cWaitForComplete();
}

#endif