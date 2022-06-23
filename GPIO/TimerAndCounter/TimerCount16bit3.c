/*
 * TimerCount16bit3.c
 *
 * Created: 2022-06-22 오전 11:51:12
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL;
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};

volatile unsigned char time_s = 0;

int main()
{
	DDRA = 0xFF;
	
	// non-PWM, 노말포트동작, 노말모드 
	TCCR1A = 0x00;
	
	// 1024 분주 = 7372800 / 1024 = 7200
	TCCR1B = (1 << CS12) |  (0 << CS11) | (1 << CS10) ;
	
	// 1초마다 인터럽트 발생
	OCR1A = 7200;
	
	// 비교출력 인터럽트 enable
	TIMSK = (1 << OCIE1A);
	TIFR |= (1 << OCF1A);
	sei();
	
	while(1)
	{
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

SIGNAL(TIMER1_COMPA_vect)
{
	cli();
	
	OCR1A = 7200;
	
	if(time_s >= 10)
	time_s = 0;
	else
	time_s++;
	
	sei();
}