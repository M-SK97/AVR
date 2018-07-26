/*
 * MPU9065_Filterd.cpp
 *
 * Created: 5/18/2018 8:29:53 PM
 * Author : legen
 */ 

#define F_CPU 16000000UL
#define BAUD 500000

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdarg.h>

#include "I2C.h"
#include "mpu9250_i2c.h"
#include "USART.h"

#define MICROSECONDS_PER_TIMER0_OVERFLOW (clockCyclesToMicroseconds(64 * 256))
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)
#define clockCyclesPerMicrosecond() ( F_CPU / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

double sampleCalc;
double *Angle;

volatile unsigned long timer0_overflow_count = 0;
volatile unsigned long timer0_millis = 0;
static unsigned char timer0_fract = 0;

unsigned long micros();
int main(void)
{
	_delay_ms(3000);
	
	TCCR0A |= 1<<WGM01 | 1<<WGM00;
	TCCR0B |= 1<<CS01 | 1<<CS00;
	TIMSK0 |= 1<<TOIE0;
	sei();
	
	initI2C();
	USART_init();
	/*while(1)
	{
		if(mpuRead8bit(WHO_AM_I_MPU9250) == 0x73)
		{
			printString("Connected");
			printString("\n");
			_delay_ms(1000);
			break;
		}
		else
		printString("MPU Offline\n");
		
		_delay_ms(200);
		
	}*/
	printString("Calibrating...");
	MPUInit();
	
	sei();
    /* Replace with your application code */
    while (1) 
    {
		PrevTime = Time;
		Time = micros();
		dt = (Time - PrevTime)/1000000.0;
		Freq = 1/dt;
		
		Angle = ComplementraryFilterMPU();
		
		char cAngleX[6];
		dtostrf(Angle[0],3,1,cAngleX);
		
		char cAngleY[6];
		dtostrf(Angle[1],3,1,cAngleY);
		
		/*char cFreq[6];
		dtostrf(Freq,3,1,cFreq);
		
		char Time[3];
		dtostrf(Freq,2,2,Time);*/
		
		/*printString(cAngleX);
		printString("\t");
		printString(cAngleY);
		printString("\t");
		printString("Freq: ");
		printString(cFreq);
		printString("\t");
		printString("Time: ");
		printString(Time);
		printString("\n");*/
		_delay_ms(4);
		
		
    }
}

ISR(TIMER0_OVF_vect)
{
	// copy these to local variables so they can be stored in registers
	// (volatile variables must be read from memory on every access)
	unsigned long m = timer0_millis;
	unsigned char f = timer0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;
	if (f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m += 1;
	}

	timer0_fract = f;
	timer0_millis = m;
	timer0_overflow_count++;
}

unsigned long micros() {
	unsigned long m;
	uint8_t oldSREG = SREG, t;
	
	cli();
	m = timer0_overflow_count;
	#if defined(TCNT0)
	t = TCNT0;
	#elif defined(TCNT0L)
	t = TCNT0L;
	#else
	#error TIMER 0 not defined
	#endif

	#ifdef TIFR0
	if ((TIFR0 & _BV(TOV0)) && (t < 255))
	m++;
	#else
	if ((TIFR & _BV(TOV0)) && (t < 255))
	m++;
	#endif

	SREG = oldSREG;
	
	return ((m << 8) + t) * (64 / clockCyclesPerMicrosecond());
}

