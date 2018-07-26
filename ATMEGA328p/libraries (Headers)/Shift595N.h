#ifndef Shift595N
#define Shift595N

#include <avr/io.h>
#include <util/delay.h>

#define	Clock	2 // port D
#define	Latch	1 // port D
#define	Data	0 // port D


void Shift(char data);
void ShiftInit(void);


void ShiftInit()
{
	DDRD |= 1<<Clock | 1<<Latch | 1<<Data;
}
void Shift(char data)
{
	for(int i=0;i<8;i++)
	{
		PORTD = ((PORTD & 0b01111111) | (data<<i & 0b10000000));
		_delay_ms(10);
		PORTD |= 1<<Clock;
		_delay_ms(10);
		PORTD &= ~(1<<Clock);
		_delay_ms(10);
		PORTD |= 1<<Latch;
		_delay_ms(10);
		PORTD &= ~(1<<Latch);
		_delay_ms(10);
	}
}




#endif