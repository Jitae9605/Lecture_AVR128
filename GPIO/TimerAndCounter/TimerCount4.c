/*
 * TimerCount4.c
 *
 * Created: 2022-06-21 오후 4:28:37
 *  Author: PKNU
 */ 
// 타이머를 이용한 디지털 시계(1)
#define F_CPU 7432800UL
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x27, 0x7f, 0x6f,0x3f};

volatile unsigned char time_s = 0;
unsigned char timer0Cnt = 0;

int main()
{
	// 포트A 출력포트로 설정
	DDRA = 0xFF;
	
	// 분주 1024 설정
	TCCR0 = (1 << CS02) | (1 << CS01) | (1 << CS00) ;
	
	// (7432800 / 1024 ) * 0.01 = 72.56...
	// 72의 상승후 인터럽트
	// 즉 256 - 74 = 182에서 시작하면 0.1초마다 한번씩 인터럽트 발생
	OCR0 = 72;
	
	// 타이머0의 출력비교 인터럽트 Enable
	TIMSK = (1 << OCIE0);
	
	// TCNT0 레지스터와 출력비교 레지스터 OCR0값을 비교해서, 같으면 이 비트가 “1”로 세트되어 출력 비교인터럽트가 발생
	TIFR  |= (1 << OCF0 );
	
	sei();
	
	while(1)
	{
		// 포트A에 FND_Tbl 값 출력
		PORTA = FND_DATA_TBL[time_s];
	}
	return 0;
	
}

// 출력 비교인터럽트
SIGNAL(TIMER0_COMP_vect)
{
	cli();
	
	OCR0 += 72;
	timer0Cnt++;
	
	// 100번 = 0.01 * 100 = 1초
	if(timer0Cnt == 100)
	{
		// time_s 가 FND_Tbl범위를 넘으면
		if(time_s >= 10)
		{
			// 초기화
			time_s = 0;
		}
		else
		{
			// 아니면 ++
			time_s++;
		}
		timer0Cnt = 0;
	}
	sei();
}