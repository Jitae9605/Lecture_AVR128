/*
 * input1.c
 *
 * Created: 2022-06-17 오후 3:20:46
 *  Author: PKNU
 */ 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	DDRD = 0x00;	// 포트D 를 입력으로 설정
	DDRA = 0xff;	// 포트A 를 출력으로 설정

	while(1)
	{
		for(int i = 0x01; i < 0x81; i *= 2)
		{
			if(PIND == i)
			{
				PORTA = i;	// 포트D 입력 값이 0x01이면 포트A의 0번 비트를 출력
				_delay_ms(300);
				PORTA = 0x00;
			}
		}

	}
}
