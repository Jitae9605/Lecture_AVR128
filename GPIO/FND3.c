/*
 * FND3.c
 *
 * Created: 2022-06-20 오전 9:25:04
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
	
	int arr_in[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	int arr_out0[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};
	int num_1 = 0;
	int num_10 = 0;
	int num_100 = 0;
	int num_1000 = 0;
	
	PORTA = arr_out0[0];
	PORTF = arr_out0[0];

	while(1)
	{

		if(PIND == arr_in[0])
		{
			_delay_ms(100);
			// 0000 0000
			// 1111 1111
			// 0 ~ 15
			// 0 1 2 3 4 5 6 7 8 9 A B C D E F
			
			num_1++;
			if(num_1 > 9)
			{
				num_1 = 0;
				num_10++;
				
				if(num_10 > 9)
				{
					num_10 = 0;
					num_100++;
					
					if(num_100 > 9)
					{
						num_100 = 0;
						num_1000++;
						
					}
					
				}
				
			}
			
		}
				PORTA = 0x07;			// 0111
				PORTF = arr_out0[num_1];
				_delay_ms(1);
				
				PORTA = 0x0B;			// 1011
				PORTF = arr_out0[num_10];
				_delay_ms(1);
				
				PORTA = 0x0D;			// 1101
				PORTF = arr_out0[num_100];
				_delay_ms(1);
				
				PORTA = 0x0E;			// 1011
				PORTF = arr_out0[num_1000];
				_delay_ms(1);
				

			
		
		

	}

	return 0;
}
