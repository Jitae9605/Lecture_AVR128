/*
 * lcdtest1.c
 *
 * Created: 2022-06-20 오후 3:08:11
 *  Author: PKNU
 */ 
#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <stdio.h>
#include "Lcd.h"

// 8비트 LCD 출력

int main()
{
	// 1. sprintf 없이 출력
	while(1)
	{
		DDRA = 0xFF;
		DDRG = 0x0F;
		
		Lcd_Init();
		Lcd_Clear();
	
		while(1)
		{
			// 2. sprintf를 이용해
			Lcd_Clear();
			unsigned char arr[20];
			for(int a = 1; a < 10; a++)
			{
				for(int b = 1; b < 10; b++)
				{
					int c = a * b;
					sprintf(arr,"%d * %d = %d", a, b, c);
					Lcd_Pos(0,0);
					Lcd_STR(arr);
					_delay_ms(500);
				}
				Lcd_Clear();
			}
			break;
		}
			
		while(1)
		{
			 for(int i = 1; i < 10; i++)
			 {
				 for(int j = 1; j < 10; j++)
				 {
					 int k = i * j;

					 Byte *gogodan = "0 * 1 = 1";
					 gogodan[0] = i + '0';
					 gogodan[4] = j + '0';
					 gogodan[8] = k/10 + '0';
					 gogodan[9] = k%10 + '0';
					 
					 Lcd_Pos(1,0);
					 Lcd_STR(gogodan);
					 _delay_ms(500);
				 }
				 Lcd_Clear();
			 }
			 break;
		}
	}
	return 0;
}