/*
 * interupt2.c
 *
 * Created: 2022-06-20 오전 11:20:33
 *  Author: PKNU
 */ 
 
// 4번 인터럽트를 이용해 하강에지로 설정해 LED점등변화 주는 실습
 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char exchage = 0;

SIGNAL(INT4_vect)
{
	exchage = ~exchage;	// 비트 not 연산자
}

int main()
{
	// 포트 입출력 설정
	DDRE = 0x00;
	DDRA = 0xff;
	
	// INT4 활성화
	EIMSK = (1 << INT4 ); 
	EICRB = (1 << ISC41) | (0 << ISC40);
	sei();
	
	
	while(1)
	{
		PORTA = 0x00;
		
		if(exchage) 
		{
			PORTA = 0xff;
			_delay_ms(300);
			exchage = 0;
		}
		
	}
	
	return 0;
}