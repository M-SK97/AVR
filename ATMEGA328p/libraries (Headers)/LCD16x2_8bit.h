/*
* The LCD.c
*
* Created: 8/22/2017 7:17:26 AM
* Author : M-SK
*/
#include <avr/io.h>
#include <util/delay.h>

#define MrLCDsCrib            PORTD
#define DataDir_MrLCDsCrib    DDRD
#define MrLCDsControl         PORTB
#define DataDir_MrLCDControl  DDRB
#define LightSwitch           1      //E
#define ReadWrite             2      //R/W
#define BiPolarMood           0      //RS

void Check_If_MrLCD_isBusy(void);
void Peek_A_Boo(void);
void Send_A_Command(unsigned char command);
void Send_A_Character(unsigned char character);
void Send_A_String(const char myString[]);
void Init_MrLCD_RecievingData();


void Peek_A_Boo()
{
	MrLCDsControl |= 1<<LightSwitch;
	asm volatile ("nop");
	asm volatile ("nop");
	MrLCDsControl &= ~1<<LightSwitch;
}
void Send_A_Command(unsigned char command)
{
	Check_If_MrLCD_isBusy();
	MrLCDsCrib = command;
	MrLCDsControl &= ~(1<<ReadWrite|1<<BiPolarMood);
	Peek_A_Boo();
	MrLCDsCrib = 0;
}
void Send_A_Character(unsigned char character)
{
	Check_If_MrLCD_isBusy();
	MrLCDsCrib = character;
	MrLCDsControl &= ~(1<<ReadWrite); //0 to R/W
	MrLCDsControl |= 1<<BiPolarMood; // 1 to Enable
	Peek_A_Boo();
	MrLCDsCrib = 0;
	
}

void Check_If_MrLCD_isBusy()
{
	DataDir_MrLCDsCrib = 0;
	MrLCDsControl |= 1<<ReadWrite;
	MrLCDsControl &= ~1<<BiPolarMood;
	
	while (MrLCDsCrib >= 0x80)
	{
		Peek_A_Boo();
	}
	
	DataDir_MrLCDsCrib = 0xFF;
}

void Send_A_String(const char* myString)
{
	uint8_t i=0;
	while(myString[i])
	{
		Send_A_Character(myString[i]);
		_delay_us(40);
		
		i++;
	}
}

void Init_MrLCD_RecievingData()
{
	DataDir_MrLCDControl |= 1<<LightSwitch | 1<<ReadWrite | 1<<BiPolarMood;
	_delay_ms(15);
	
	
	Send_A_Command(0x01);
	_delay_ms(2);
	
	Send_A_Command(0x38);
	_delay_us(50);
	Send_A_Command(0b00001110);
	_delay_us(50);
}