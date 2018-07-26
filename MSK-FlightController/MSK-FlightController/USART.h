#ifndef USART
#define USART

//By M-SK.
#include <util/setbaud.h>
#include <stdlib.h>

void USART_init(void)
{
	//Set baud rate.
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	//Enable TX and RX.
	UCSR0B = (1<<TXEN0) | (1<<RXEN0);
	
	//Set frame format.
	UCSR0C = (0<<UCSR0B) | (3<<UCSZ00);
}

//Sending Byte function.
void USART_transmitByte(unsigned char data)
{
	//Check if "UDR0" register is empty.
	loop_until_bit_is_set(UCSR0A,UDRE0);
	
	//Write to "UDR0" register.
	UDR0 = data;
}

//Receiving Byte function.
unsigned char USART_receiveByte(void)
{
	//Wait till for Byte to be received in "UDR0" register.
	loop_until_bit_is_set(UCSR0A,RXC0);
	
	//Return the Byte that is in the "UDR0" register.
	return UDR0;
}

//Sending a string function.
void printString(const char C[])
{
	uint8_t i=0;
	while(C[i])
	{
		USART_transmitByte(C[i]);
		i++;
	}
	
}

void printInt(int value , int nOfDigits)
{
	char nToString[nOfDigits];
	itoa(value, nToString, 10);
	
	printString(nToString);
	//LCD_print(" ");

}

#endif