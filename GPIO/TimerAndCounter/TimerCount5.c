/*
 * TimerCount5.c
 *
 * Created: 2022-06-21 오후 4:57:10
 *  Author: PKNU
 */ 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};

volatile unsigned char time_s = 0;
unsigned char Time_STOP = 0;

unsigned char timer0Cnt = 0;

int main()
{
	DDRA = 0xFF;
	DDRE = 0x00;
	
	TCCR2 = (1 << CS22) | ( 1 << CS20);
	
	OCR2 = 72;
	
	// 타이머2, 출력 비교 인터럽트
	TIMSK = (1 << OCIE2);
	
	TIFR |= (1 << OCF2);
	
	// 인터럽트 4를 상승엣지
	EIMSK = (1 << INT4);
	EICRB = (1 << ISC40) | (1 << ISC41);
	EIFR = (1 << INTF4);
	
	sei();
	
	while(1)
	{
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
}

SIGNAL(TIMER2_COMP_vect)
{
	cli();
	OCR2 = 72;
	timer0Cnt++;
	
	if(timer0Cnt == 50)
	{
		if(Time_STOP == 0)
		{
			if(time_s >= 10)
				time_s = 0;
			else
				time_s++;
		}
		
		timer0Cnt = 0;
	}
	sei();	
}

SIGNAL(INT4_vect)
{
	cli();
	
	if(Time_STOP == 0)
		Time_STOP = 1;
	else
		Time_STOP = 0;
	
	sei();
}