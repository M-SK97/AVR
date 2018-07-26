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
#include "micros.h"

int receiver_input[4];
double sampleCalc;
double *Angle;
long timer_0, timer_1, timer_2, timer_3;
unsigned long esc_timer = 0;
unsigned long last_channel_0=0, last_channel_1=0, last_channel_2=0, last_channel_3=0;
int main(void)
{
	_delay_ms(3000);
	PCICR |= 1<<PCIE1;
	PCMSK1 |= 1<<PCINT8 | 1<<PCINT9;
	microsinit();
	initI2C();
	USART_init();
	MPUInit();
	
	DDRD |= 1<<7 | 1<<6 | 1<<5 | 1<<4;;
    /* Replace with your application code */
    while (1) 
    {
		
		/*PORTD |= 1<<7;
		Angle = ComplementraryFilterMPU();
		
		esc_timer = micros()+1000;
		PORTB |= 1<<0;
		PORTD |= 1<<6 | 1<<5 | 1<<4;
		while(micros()<esc_timer);
		PORTB &= ~(1<<0);
		PORTD &= ~(1<<6);
		PORTD &= ~(1<<5);
		PORTD &= ~(1<<4);
		
		char cAngleX[6];
		dtostrf(Angle[0],3,1,cAngleX);
		
		char cAngleY[6];
		dtostrf(Angle[1],3,1,cAngleY);
		
		printString(cAngleX);
		printString("\t");
		printString(cAngleY);
		printString("\n");
		PORTD &= ~(1<<7);*/
		
		char cAngleX[6];
		dtostrf(receiver_input[0]/1.0,5,0,cAngleX);
		
		char cAngleY[6];
		dtostrf(receiver_input[1]/1.0,5,0,cAngleY);
		
		printString(cAngleX);
		printString("\t");
		printString(cAngleY);
		printString("\n");
		
		if(receiver_input[0]>=1504 && receiver_input[0]<=1496)
		{
			esc_timer = micros()+receiver_input[0];
			PORTD |= 1<<7 | 1<<6 | 1<<5;
			while(micros()<esc_timer);
			PORTD &= ~(1<<5);
			PORTD &= ~(1<<6);
			PORTD &= ~(1<<7);
		}
		else if (receiver_input[0]>1504)
		{
			esc_timer = micros()-receiver_input[0];
			PORTD &= ~(1<<7);
			while(micros()<esc_timer);
			PORTD |= (1<<5);
			PORTD |= (1<<6);
			PORTD |= (1<<7);
		}
		else if (receiver_input[0]<1496)
		{
			esc_timer = micros()+receiver_input[0];
			PORTD &= ~(1<<6);
			while(micros()<esc_timer);
			PORTD |= (1<<6);
			PORTD |= (1<<5);
			PORTD |= (1<<7);
		}
		
		
    }
	
	

}

ISR(PCINT1_vect)
{
	Time = micros();
	//Channel 1=========================================
	if(PINC & 1<<0)										//Check if input is high
	{
		if(last_channel_0 == 0)							//Check if the last channel was low
		{
			last_channel_0 = 1;							//Remember the current pin state
			timer_0 = Time;								//Set The first time of the change
		}
	}
	else if(last_channel_0 == 1)						//Check if last channel was high
	{
		last_channel_0 = 0;								//Remember the current pin state
		receiver_input[0] = Time - timer_0;				//Subtract the first time from the current time
	}
	//Channel 2=========================================
	if(PINC & 1<<1)										//Check if input is high
	{
		if(last_channel_1 == 0)							//Check if the last channel was low
		{
			last_channel_1 = 1;							//Remember the current pin state
			timer_1 = Time;								//Set The first time of the change
		}
	}
	else if(last_channel_1 == 1)						//Check if last channel was high
	{
		last_channel_1 = 0;								//Remember the current pin state
		receiver_input[1] = Time - timer_1;				//Subtract the first time from the current time
	}
	//Channel 3=========================================
	if(PINC & 1<<2)										//Check if input is high
	{
		if(last_channel_2 == 0)							//Check if the last channel was low
		{
			last_channel_2 = 1;							//Remember the current pin state
			timer_2 = Time;								//Set The first time of the change
		}
	}
	else if(last_channel_2 == 1)						//Check if last channel was high
	{
		last_channel_2 = 0;								//Remember the current pin state
		receiver_input[2] = Time - timer_2;				//Subtract the first time from the current time
	}
	//Channel 4=========================================
	if(PINC & 1<<3)										//Check if input is high
	{
		if(last_channel_3 == 0)							//Check if the last channel was low
		{
			last_channel_3 = 1;							//Remember the current pin state
			timer_3= Time;								//Set The first time of the change
		}
	}
	else if(last_channel_3 == 1)						//Check if last channel was high
	{
		last_channel_3 = 0;								//Remember the current pin state
		receiver_input[3] = Time - timer_3;				//Subtract the first time from the current time
	}
}