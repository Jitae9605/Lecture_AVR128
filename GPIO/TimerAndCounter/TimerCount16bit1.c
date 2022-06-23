/*
 * TimerCount_16bit1c.c
 *
 * Created: 2022-06-22 오전 10:15:30
 *  Author: PKNU
 */ 
//타이머로 LED 점멸(1)
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_data = 0x00;
unsigned char timer1Cnt = 0;

int main()
{
	// 출력포트 설정
	DDRC = 0xFF;
	
	// non-PWM, 노말포트동작, 노말모드 
	TCCR1A = 0x00;
	
	// 256 분주 = 7372800 / 256 = 28800
	TCCR1B = (1 << CS12) |  (0 << CS11) | (0 << CS10) ;
	
	// 레지스터 시작점 설정
	// 16비트는 65535가 넘어가면 오버플로우 인터럽트 발생
	// 0.01초에 클럭수를 통해 계산하면 (7372800 / 256 = 28800) 288번의 값 상승이 있다
	// 즉, 65536 - (0.01*(7372800/256)) = 65248
	// 시작점을 65248로 하면 0.01초 마다 인터럽트가 발생한다.
	TCNT1 = 65248;
	
	// 타이머1의 오버플로우 인터럽트를 개별적으로 Enable
	TIMSK = (1 << TOIE1);
	
	// 오버플로우가 발생하면(0xFFFF까지 세고 0x0000으로 넘어가게 되면) 1로 세트되면서 오버플로우 인터럽트가 발생
	TIFR |= (1 << TOV1); // 플래그임
	
	sei();
	
	while(1)
	{
		PORTC = LED_data;
	}
	return 0;
}

SIGNAL(TIMER1_OVF_vect)
{
	cli();
	TCNT1 = 65248;
	timer1Cnt++;
	
	if(timer1Cnt == 100)
	{
		LED_data++;
		
		if(LED_data > 0xFF)
			LED_data = 0;
		
		timer1Cnt = 0;
	}
	
	sei();
}