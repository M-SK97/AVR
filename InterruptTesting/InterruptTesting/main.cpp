/*
 * InterruptTesting.cpp
 *
 * Created: 9/5/2017 8:49:03 AM
 * Author : legen
 */ 

#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include "LCD16x2_4bit.h"
#include "USART.h"
#include <avr/interrupt.h>

int c;
int main(void)
{
	USART_init();
	LCD_Init();
	//TCCR1A = 0;
	//TCCR1B |= 1<<WGM12 | 1<<CS12 | 1<<CS10;
	EICRA |= 1<<ISC01; //11 for rising -- 10 for falling
	EIMSK |= 1<<INT0;
	//TIMSK1 |= 1<<OCIE1A;
	//OCR1A = 15625;
	sei();
	c=0;
    /* Replace with your application code */
    while (1) 
    {
			LCD_printInt_toLocation(0,0,c,2);
			printInt(c,2);
			printString("\n");
    }
}

ISR(INT0_vect)
{
	c++;
}

/*ISR(INT0_vect)
{
	c = TCNT1;
*/

