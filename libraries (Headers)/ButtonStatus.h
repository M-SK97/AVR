#ifndef ButtonStatus
#define ButtonStatus

#include <avr/io.h>
#include <stdlib.h>

void debounce();
char ButtonPressed(int pin , int pinPort , int confedenceCounter);

int pressed = 0;
int PressedconfedenceCounter = 0;
int UnpressedconfedenceCounter = 0;

char ButtonPressed(int pin , int pinPort , int confedenceCounter)
{
	if(bit_is_clear(pin,pinPort))
	{
		UnpressedconfedenceCounter = 0;
		PressedconfedenceCounter++;
		if(PressedconfedenceCounter > confedenceCounter)
		{
			if(pressed == 0)
			{
				pressed = 1;
				return 1;
			}
			PressedconfedenceCounter = 0;
		}
	}
	
	else
	{
		return 0;
	}
	
	return -1;
}
#endif