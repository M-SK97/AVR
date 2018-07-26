#ifndef LCD16x2
#define LCD16x2

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define LCDPort				PORTD
#define LCDPort2			PORTB
#define DataDir_LCDPort		DDRD
#define LCDControlPort		PORTB
#define DataDir_LCDControl	DDRB
#define Enable				4		//E
#define ReadWrite			5		//R/W
#define RigisterSelect		0		//RS

void LCD_Init (void);
void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_print(const char myString[]);
void LCD_clear(void);
void LCD_shift_to_left(int steps, int Time);
void LCD_shift_to_right(int steps, int Time);
void cursor_shift_to_left(int steps, int Time);
void cursor_shift_to_right(int steps, int Time);
void Delay_ms(int Time);
void LCD_home(void);
void cursor_entry_shift_to_left(int steps);
void cursor_entry_shift_to_right(int steps);
void LCD_entry_shift_to_left(int steps);
void LCD_entry_shift_to_right(int steps);
void LCD_set_DDRAM_address(int pos);
void LCD_first_line(void);
void LCD_second_line(void);
void LCD_go_to_location(uint8_t x, uint8_t y);
void LCD_print_ToLocation(uint8_t x, uint8_t y, const char myString[]);
void LCD_printInt_toLocation(uint8_t x, uint8_t y, int value, int nOfDigits);

void LCD_Init ()  /* LCD Initialize function */
{
	DataDir_LCDPort = 0xFF; /* Make LCD port direction as o/p */
	DataDir_LCDControl |= 1<<Enable | 1<<ReadWrite | 1<<RigisterSelect | 1<<3 | 1<<4;
	
	DataDir_LCDControl = 0xFF;
	
	_delay_ms(20);		/* LCD Power ON delay always >15ms */
	
	LCD_Command(0x01); // clear the LCD.
	_delay_ms(2);
	
	LCD_Command(0x33);
	LCD_Command(0b00110010);	/* Send for 4 bit initialization of LCD  */
	LCD_Command(0b00101000);	/* 2 line, 5*7 matrix in 4-bit mode */
	LCD_Command(0b00001100);	/* Display on cursor off */
	LCD_Command(0x06);	/* Increment cursor (shift cursor to right) */
	LCD_Command(0x01); // clear the LCD.
	_delay_ms(2);
}

void LCD_Command( unsigned char cmnd )
{
	LCDPort = (LCDPort & 0x0F) | (cmnd & 0xF0); //Sending upper nibble.
	
	
	/* // Here im checking if i can put the middle wires DB5 and DB6 in PB3 and PB4.
	MrLCDsCrib = (MrLCDsCrib & 0b01101111) | (cmnd & 0b10010000);// Sending upper nibble 1st 1st two.
	MrLCDsCrib2 = (MrLCDsCrib2 & 0b11100111) | ((cmnd & 0b01100000)>>2); // Sending upper nibble 2nd 1st two.
	*/
	
	LCDControlPort &= ~ (1<<RigisterSelect);		/* RS=0, command reg. */
	LCDControlPort |= (1<<Enable);		/* Enable pulse */
	_delay_us(1);
	LCDControlPort &= ~ (1<<Enable);
	_delay_us(200);
	
	LCDPort = (LCDPort & 0x0F) | (cmnd << 4);  // Sending lower nibble.
	
	/* // Here im checking if i can put the middle wires DB5 and DB6 in PB3 and PB4.
	MrLCDsCrib = (MrLCDsCrib & 0b01101111) | ((cmnd & 0b00000110) << 4); // Sending lower nibble 1st 2nd two 
	MrLCDsCrib2 = (MrLCDsCrib2 & 0b11100111) | ((cmnd & 0b00000110) << 4); // Sending lower nibble 2nd 2nd two 
	*/
	
	LCDControlPort|= (1<<Enable);
	_delay_us(1);
	LCDControlPort &= ~ (1<<Enable);
	_delay_ms(2);
}

void LCD_Char( unsigned char data )
{
	LCDPort = (LCDPort & 0x0F) | (data & 0xF0); // Sending upper nibble.
	
	/* //Here im checking if i can put the middle wires DB5 and DB6 in PB3 and PB4.
	MrLCDsCrib = (MrLCDsCrib & 0b01101111) | (data & 0b10010000);// Sending upper nibble 1st 1st two.
	MrLCDsCrib2 = (MrLCDsCrib2 & 0b11100111) | ((data & 0b01100000)>>2); // Sending upper nibble 2nd 1st two.
	*/
	
	
	LCDControlPort |= (1<<RigisterSelect);  /* RS=1, data reg. */
	LCDControlPort|= (1<<Enable);
	_delay_us(1);
	LCDControlPort &= ~ (1<<Enable);
	_delay_us(200);
	
	LCDPort = (LCDPort & 0x0F) | (data << 4);  // Sending lower nibble.
	
	/* // Here im checking if i can put the middle wires DB5 and DB6 in PB3 and PB4.
	
	MrLCDsCrib = (MrLCDsCrib & 0b01101111) | ((data & 0b00000110) << 4);/ Sending lower nibble 1st 2nd two 
	MrLCDsCrib2 = (MrLCDsCrib2 & 0b11100111) | ((data & 0b00000110) << 4);/ Sending lower nibble 2nd 2nd two 
	
	*/
	
	LCDControlPort |= (1<<Enable);
	_delay_us(1);
	LCDControlPort &= ~ (1<<Enable);
	_delay_ms(2);
}

void LCD_print(const char myString[])
{
	uint8_t i = 0;
	
	while(myString[i])
	{
		LCD_Char(myString[i]);
		_delay_us(40);
		
		i++;
	}
}

void LCD_clear()
{
	LCD_Command(0x01);
	_delay_ms(2);
}

void LCD_shift_to_left(int steps, int Time)
{
	for(int i=0;i<steps;i++)
	{
		LCD_Command(0b00011000);
		Delay_ms(Time);
	}
}

void Delay_ms(int Time)
{
	for (int i=0;i<Time;i++)
	{
		_delay_ms(1);
	}
}

void LCD_shift_to_right(int steps, int Time)
{
	for(int i=0;i<steps;i++)
	{
		LCD_Command(0b00011100);
		Delay_ms(Time);
	}
}

void cursor_shift_to_left(int steps, int Time)
{
	for(int i=0;i<steps;i++)
	{
		LCD_Command(0b00010000);
		_delay_us(40);
		Delay_ms(Time);
	}
}

void cursor_shift_to_right(int steps, int Time)
{
	for(int i=0;i<steps;i++)
	{
		LCD_Command(0b00010100);
		Delay_ms(Time);
	}
}

void LCD_home()
{
	LCD_Command(0b00000010);
	_delay_ms(2);
}

void cursor_entry_shift_to_left(int steps)
{
	for(int i=0;i<steps;i++)
	{
		LCD_Command(0b00000100);
		_delay_us(40);
	}
}

void cursor_entry_shift_to_right(int steps)
{
	LCD_Command(0b00000110);
	_delay_us(40);
}

void LCD_entry_shift_to_left(int steps)
{
	LCD_Command(0b00000101);
	_delay_us(40);
}

void LCD_entry_shift_to_right(int steps)
{
	LCD_Command(0b00000111);
	_delay_us(40);
}

void LCD_set_DDRAM_address(int pos)
{
	pos += 0b10000000;
	
	LCD_Command(pos);
	_delay_us(40);
}

void LCD_first_line()
{
	LCD_set_DDRAM_address(0);
} 

void LCD_second_line(void)
{
	LCD_set_DDRAM_address(64);
}

void LCD_go_to_location(uint8_t x, uint8_t y)
{
	int numberOfRows[2] = {0,64};
		
	LCD_set_DDRAM_address(numberOfRows[y] + (x));
}

void LCD_print_ToLocation(uint8_t x, uint8_t y, const char myString[])
{
	LCD_go_to_location(x, y);
	LCD_print(myString);
}

void LCD_printInt_toLocation(uint8_t x, uint8_t y, int value, int nOfDigits)
{
	char nToString[nOfDigits];
	itoa(value, nToString, 10);
	
	LCD_print_ToLocation(x, y, nToString);
	//LCD_print(" ");

}

#endif