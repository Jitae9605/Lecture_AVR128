/*
* FND2.c
*
* Created: 2022-06-17 오후 4:57:07
*  Author: PKNU
*/


#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRA = 0xFF;
	DDRF = 0xFF;
	DDRD = 0x00;
	DDRB = 0xFF;
	
	int arr_in[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	int arr_out0[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};
	int i = 0;
	int j = 0;
	int k = 0xFF;
	
	PORTA = arr_out0[0];
	PORTF = arr_out0[0];

	while(1)
	{

		if(PIND == arr_in[0])
		{
			PORTB = k;
			PORTA = arr_out0[j];
			PORTF = arr_out0[i];
			i++;
			
			if(i > 9)
			{
				i = 0;
				j++;
			}
			
			if(i > 9 && j > 9)
			{
				i = 0;
				j = 0;
			}
			_delay_ms(300);
		}
		

	}

	return 0;
}
