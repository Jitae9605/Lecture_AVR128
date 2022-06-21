/*
 * TimerCount.c
 *
 * Created: 2022-06-21 오후 3:31:01
 *  Author: PKNU
 */ 
//타이머로 LED 점멸(1)
#define F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;
unsigned char timer0Cnt = 0;

int main()
{
	// 포트C 출력포트로 설정
	DDRC = 0xFF;	
	
	// 분주 1024 설정
	TCCR0 = (1 << CS02) | (1 << CS01) | (1 << CS00) ;
	
	// (7432800 / 1024 ) * 0.01 = 72.56... 
	// 72의 상승후 인터럽트 
	// 즉 255 - 74 = 182에서 시작하면 0.1초마다 한번씩 인터럽트 발생
	TCNT0 = 182;
	
	// 타이머 오버플로우 인터럽트 Enable
	TIMSK |= (1 << TOIE0);
	
	// 타이머 버플로우가 발생하면 이 비트가 “1”로 세트되어 오버플로우 인터럽트가 발생
	TIFR  |= (1 << TOV0 );
	sei();
	
	while(1)
	{
		// 포트C로 변수 LED_Data에 있는 데이터 출력
		PORTC = LED_Data;
	}	
}

SIGNAL(TIMER0_OVF_vect)
{
	cli();
	
	// 0.01초마다 인터럽트 발생
	TCNT0 = 182;
	timer0Cnt++;
	
	// 0.01 * 100 = 1초
	if(timer0Cnt == 100)
	{
		// LED Data를 증가시킴
		LED_Data++;
		
		// 16지수 LED점등 출력 범위를 넘어서면
		if (LED_Data > 0xFF)
		{
			// 초기화
			LED_Data = 0;
		}
		
		// 1초가 되어 기능을 수행했으므로 다시 0초로 돌려 1초를 센다(0.01초 100번)
		timer0Cnt = 0;
	}
}