#ifndef Clock
#define Clock

#include "LCD16x2_4bit.h"
#include "USART.h"

class CLOCK
{
	private:
	
	int h;
	int m;
	int s;
	
	public:
	
	void getTime(int &hour , int &min , int &sec);
	void setTime(const int hour ,const int min ,const int sec);
	void increamentTime(void);
	
};

/*CLOCK::CLOCK(int hour , int min , int sec)
{
	
}*/

void CLOCK::getTime(int &hour , int &min , int &sec)
{
	hour = h;
	min = m;
	sec = s;
}

void CLOCK::setTime(const int hour ,const int min ,const int sec)
{
	h = hour;
	m = min;
	s = sec;
}
void CLOCK::increamentTime()
{
	if(s<59)
	{
		
		s++;
	}
	else if(s==59 && m<59)
	{
		
		s=0;
		m++;
	}

	else if(s==59 && m==59 && h<23)
	{
		
		s=0;
		m=0;
		h++;
	}

	else if(s==59 && m==59 && h==23)
	{
		
		s=0;
		m=0;
		h=0;
	}
}



#endif