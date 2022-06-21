/*
 * interupt5.c
 *
 * Created: 2022-06-20 오후 12:04:45
 *  Author: PKNU
 */ 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char Time_Stop = 0;
volatile unsigned char cnt = 0;

int main()
{
	unsigned char FND_DATA_TBL[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0X7C,0X07,0X7F,0X67};
	
	DDRA = 0xff;
	DDRE = 0x00;
	
	// 인터럽트 4 = 하강에지, 6 = 상승에지
	EICRB =   ( 1 << ISC41 ) | ( 0 << ISC40 )
			| ( 1 << ISC61 ) | ( 1 << ISC60 );
	EIMSK =	  ( 1 << INT4 ) 
			| ( 1 << INT6 );
	sei();
				
		 
	while(1)
	{
		PORTA = FND_DATA_TBL[cnt];
		if(Time_Stop == 0)
		{
			cnt++;
			if(cnt > 9) cnt = 0;
		}
		_delay_ms(200);
	}
	
	return 0;
}

SIGNAL(INT4_vect)
{
	cli();
	Time_Stop = 1;
	cnt = 0;
	sei();
}

SIGNAL(INT6_vect)
{
	cli();
	Time_Stop = 0;
	cnt = 0;
	sei();
}
