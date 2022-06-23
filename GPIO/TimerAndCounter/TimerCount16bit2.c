/*
 * TimerCount16bit2.c
 *
 * Created: 2022-06-22 오전 11:38:37
 *  Author: PKNU
 */ 
//프리스케일러를 변경하여 타이머로 LED 점멸(2)
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned char LED_Data = 0x00;

int main()
{
	DDRC = 0xFF;
	
	
	// non-PWM, 노말포트동작, 노말모드 
	TCCR3A = 0x00;
	
	// 1024 분주 = 7372800 / 1024 = 7200
	TCCR3B = (1 << CS32) |  (0 << CS31) | (1 << CS30) ;
	
	// 65536 - 7200 = 58336 -> 1초마다 인터럽트 발생
	TCNT3 = 58336;
	
	// 타이머1의 오버플로우 인터럽트를 개별적으로 Enable
	ETIMSK = (1 << TOIE3);
	
	// 오버플로우가 발생하면(0xFFFF까지 세고 0x0000으로 넘어가게 되면) 1로 세트되면서 오버플로우 인터럽트가 발생
	ETIFR |= (1 << TOV3); // 플래그임
	
	sei();
	
	while(1)
	{
		PORTC = LED_Data;
	}
	return 0;
	
	
}

SIGNAL(TIMER3_OVF_vect)
{
	cli();
	
	TCNT3 = 58336;
	LED_Data++;
	
	if(LED_Data > 0xFF)
		LED_Data = 0;
	sei();
}