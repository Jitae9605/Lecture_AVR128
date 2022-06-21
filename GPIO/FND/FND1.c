/*
 * FND.c
 *
 * Created: 2022-06-17 오후 4:21:11
 *  Author: PKNU
 */ 

#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRA = 0xFF;
	DDRD = 0x00;
	
	int arr_in[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
	int arr_out0[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x27,0x7f,0x6f};
	
	PORTA = 0x3f;
	while(1)
	{	
		for(int i = 0; i < 8; i++)
		{
			if(PIND == arr_in[i])
			{
				PORTA = arr_out0[i+1];
			}
			
		
		}	

	}
	
	return 0;
}
