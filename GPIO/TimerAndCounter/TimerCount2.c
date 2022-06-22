/*
 * TimerCount2.c
 *
 * Created: 2022-06-21 오후 3:58:26
 *  Author: PKNU
 */ 
//타이머로 LED 점멸(2)
#define F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x01;
unsigned char timer2Cnt = 0;
unsigned char Shift_Flag = 0;

int main()
{
	DDRC = 0xFF;
	
	TCCR2 = (1 << CS22) | (1 << CS20);
	
	
	// 256 - (7432800 / 1024) = 183
	// 타이머2 의 카운트 시작위치설정 
	TCNT2 = 183;
	
	// 타이머2 오버플로우 인터럽트 Enable
	TIMSK = (1 << TOIE2);
	TIFR |= (1 << TOV2);
	
	sei();
	
	while(1)
	{
		PORTC = LED_Data;
	}
	return 0;
}

// 타이머 오버플로우 인터럽트
SIGNAL(TIMER2_OVF_vect)
{
	cli();
	
	// 256 - (7432800 / 1024) = 183
	// 타이머2 의 카운트 시작위치설정
	TCNT2 = 183;
	timer2Cnt++;
	
	// 0.01초마다 timer2Cnt가 증가하므로
	// timer2Cnt == 50 => 0.5초를 센것!
	if(timer2Cnt == 50)
	{
		
		if(Shift_Flag == 0)
		{
			// LED 0 -> 8 방향으로 LED점등 쉬프트
			LED_Data <<= 1;	
			
			// LED 8로 이동되면 반대방향으로 이동하게 설정
			if(LED_Data == 0x80)
				Shift_Flag = 1;
		}
		else
		{
			// LED 8 -> 0 방향으로 LED점등 쉬프트
			LED_Data >>= 1;
			
			// LED 0으로 이동되면 반대방향으로 이동하게 설정
			if(LED_Data == 0x01)
				Shift_Flag = 0;
		}
		
		// 초기화
		timer2Cnt = 0;
	}
	sei();
}