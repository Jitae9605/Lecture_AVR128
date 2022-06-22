/*
 * led2.c
 *
 * Created: 2022-06-17 오후 1:54:32
 *  Author: PKNU
 */ 

#define F_CPU 7372800
#include <avr/io.h>
#include <util/delay.h>


int main()
{
	DDRA = 0xff;
	DDRB = 0xff;
	DDRC = 0xff;
	

	while(1)
	{
		// 다 꺼짐	  = 0x00 = 00000000 

		// 1번만 켜짐 = 0x01 = 00000001
		// 2번만 켜짐 = 0x02 = 00000010
		// 3번만 켜짐 = 0x04 = 00000100
		// 4번만 켜짐 = 0x08 = 00001000
		// 5번만 켜짐 = 0x10 = 00010000
		// 6번만 켜짐 = 0x20 = 00100000
		// 7번만 켜짐 = 0x40 = 01000000
		// 8번만 켜짐 = 0x80 = 10000000

		// 다 켜짐     = 0xFF = 11111111

		// 0  1  2  3  4  5  6  7  8  9   A    B   C   D   E   F
		//                               10   11  12  13  14  15
		
		PORTC = 0xFF;
		_delay_ms(1000);

		int c = 0x01;
		for(int i = 0; i < 8; i++)
		{
			
			int b = 0xFF;
			b -= c;
			PORTC = b;
			_delay_ms(1000);
			c *= 2;
			
		}

		PORTC = 0xFF;
		_delay_ms(1000);

		int d = 0x80;
		for(int i = 0; i < 8; i++)
		{
			int b = 0xFF;
			b -= d ;
			PORTC = b;
			_delay_ms(1000);
			d /= 2;
		}
	}

	return 0;
}
