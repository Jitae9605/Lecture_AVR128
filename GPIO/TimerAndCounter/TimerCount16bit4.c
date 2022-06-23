/*
 * TimerCount16bit4.c
 *
 * Created: 2022-06-22 오후 1:53:46
 *  Author: PKNU
 */ 
// 타이머를 이용한 스위치 누른시간 구하기
#define F_CPU 7372800UL;
#include <avr/io.h>
#include <avr/interrupt.h>

unsigned char FND_DATA_TBL[] = {0x3F,0X06,0X5B,0X4F,0X66,0X6D,0X7C,0X07,0X7F,0X67,0X77,0X7C,0X39,0X5E,0X79,0X71,0X08,0X80};
	
volatile unsigned char time_s = 0;
volatile unsigned char FND_flag = 0;
volatile unsigned char edge_flag = 0;

int main()
{
	
	DDRA = 0xFF;
	DDRE = 0x00;
	
	// non-PWM, 노말포트동작, 노말모드
	TCCR1A = 0x00;
	
	// 1024 분주 = 7372800 / 1024 = 7200
	TCCR3B = (1 << CS32) |  (0 << CS31) | (1 << CS30) ;
	
	// 1초마다 인터럽트 발생
	OCR1A = 7200;
	
	// 입력캡쳐 인터럽트 3 enable
	ETIMSK = (1 << TICIE3);
	ETIFR |= (1 << ICF3);

	sei();
	
	PORTA = FND_DATA_TBL[0];
	while(1)
	{
		if(FND_flag)
		{
			// 최대 1.5초까지 표시(F)
			if(time_s > 15)
				time_s = 15;
				
			PORTA = FND_DATA_TBL[time_s];
			FND_flag = 0;
		}
	}
	
	return 0;
}

// 신호변화를 감지 
// 평상시 -> 눌림 -> 손뗌 -> 평상시 의 루틴 사이( -> ) 에서 발생
SIGNAL(TIMER3_CAPT_vect)
{
	cli();
	
	unsigned int count_check;
	
	// 스위치가 눌린 시간 측정을 위해
	// 상승에지에서 하강에지까지의 시간을 계산
	
	
	// 상승 에지(스위치를 누르면)
	if(edge_flag == 0)
	{
		// 프리스케일러 1024, 하강에지(0 << ICES3) 캡처 트리거 설정
		TCCR3B = (1 << CS32) |  (0 << CS31) | (1 << CS30);
		
		// TCNT3 레지스터를 0으로 초기화
		TCNT3 = 0;
		
		// ICR3 레지스터를 0으로 초기화
		ICR3 = 0;
		
		edge_flag = 1;
	}
	
	// 하강 에지(스위치를 떼면)
	else
	{
		// 프리스케일러 1024, 상승에지(1 << ICES3) 캡처 트리거 설정 (카운터 값은  ICR3에 저장됨)
		TCCR3B = (1 << ICES3) | (1 << CS32) |  (0 << CS31) | (1 << CS30);
		
		// 저장된 카운터값을 변수에 입력
		count_check = ICR3;
		
		// 7372800 / 1024 = 7200, 1초동안 TCNT = 7200
		time_s = count_check / 720; // 누를시간을 0.1초 단위로 변경
		
		// 측정시간 FND로 출력 (0 ~ 1.5초 까지 측정가능)
		FND_flag = 1;
		edge_flag = 0;
		
	}
	sei();
}