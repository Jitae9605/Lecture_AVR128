/*
 * interupt1.c
 *
 * Created: 2022-06-20 오전 10:01:47
 *  Author: PKNU
 */
 
// 4번 인터럽트를 이용해 하강에지로 설정해 LED점등변화 주는 실습
 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char count = 0;

SIGNAL(INT4_vect)
{
	// 초기값 설정
	if(count == 0)
	{
		PORTA = 0x01;
	}
	
	count++;
	_delay_ms(300);
	
	PORTA *= 2;		 
	
	if(count >= 8 )
	{
		count = 0;
	}
}

int main()
{
	// 포트 입출력 설정
	DDRE = 0x00;
	DDRA = 0xff;
	
	// INT4 활성화
	EIMSK = (1 << INT4 ); 
	
	// EICRA = 0x03; // 0000 0000
	EICRB = (1 << ISC41) | (0 << ISC40) ; // 위의 (EICRA = 0x03)와 완전히 같은 것! 표현만 다른 것이다.
	// 이 방법은 한눈에 어떤 비트를 활성화 시켰는지 알수 있어 매우 좋은 코드다!
	
	sei();
	
	
	while(1)
	{
		
	}
	
	return 0;
}