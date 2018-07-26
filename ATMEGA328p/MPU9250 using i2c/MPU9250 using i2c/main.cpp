/*
 * MPU9250 using i2c.cpp
 *
 * Created: 9/29/2017 1:50:41 AM
 * Author : legen
 */ 

#define F_CPU 16000000UL
#define BAUD 9600

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "I2C.h"
#include "LCD16x2_4bit.h"
#include "mpu9250_i2c.h"
//#include "USART.h"

#define xAScale -1695.82
#define yAScale -1684.81
#define zAScale -1768.80

int main(void)
{
	initI2C();
	LCD_Init();
	//USART_init();
	
		while(1)
		{
		LCD_print("MPU Offline.");
		
		if(mpuRead8bit(WHO_AM_I_MPU9250) == 0x73)
			{
				LCD_clear();
				LCD_print("MPU Online.");
				_delay_ms(3000);
				LCD_clear();
	
				break;
			}
		_delay_ms(500);
	
		}
    /* Replace with your application code */
    while (1) 
    {
		//for(int i=0;i<3;i++)
		//{
		
		LCD_print_ToLocation(0,0,"X=");
		
		
		LCD_printDouble_toLocation(0,1,mpuRead16bit(ACCEL_XOUT_H)/xAScale,3,1,true);
		
		LCD_print_ToLocation(5,0,"Y=");
		
		
		LCD_printDouble_toLocation(5,1,mpuRead16bit(ACCEL_YOUT_H)/yAScale,3,1,true);
		
		LCD_print_ToLocation(10,0,"Z=");
		
		
		LCD_printDouble_toLocation(10,1,mpuRead16bit(ACCEL_ZOUT_H)/zAScale,3,1,true);
		
		_delay_ms(100);
		//}
		
		

    }
	return 0;
}

