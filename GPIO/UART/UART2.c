/*
 * UART2.c
 *
 * Created: 2022-06-21 오전 10:50:35
 *  Author: PKNU
 */ 
// UART로 스위치 입력 값 보내기
#define F_CPU 7372800UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile unsigned char TX_flag = 0;
volatile char TX_data = 0;	// 스위치 전송 값 저장 변수

void putch(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0)));	//  전송준비 될때까지 대기
	UDR0 = data;				// 데이터를 UDR0에 쓰면 바로 전송됨
	UCSR0A |= (1 << UDRE0);
	
}

// 문자열을 출력하는 함수
void putch_Str(char *str)
{
	
	unsigned char i = 0;
	while(str[i] != '\0')
	putch(str[i++]);	// 문자열 출력
}

int main()
{
	
	DDRE = 0x02;  // RX(입력 0), TX(출력 1), 
	
	// 플래그 0으로 초기화
	UCSR0A = 0x00;
	
	// RX,TX enable
	UCSR0B = (1 << TXEN0) | (1 << RXEN0); 
	
	// 비동기 방식, no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); 
	
	// 115200 bps
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	// 인터럽트 4,5,6,7 을 상승엣지로 동작하게 설정
	EICRB = 0xFF; 
	
	// 인터럽트 4,5,6,7 허용
	EIMSK = 0xF0;
	
	// 인터럽트 4,5,6,7 플래그를 클리어
	EIFR = 0xF0;
	
	sei();
	
	while(1)
	{
		if(TX_flag == 1)
		{
			putch_Str("\r\n Input Switch : ");
			putch(TX_data);
			TX_flag = 0;
		}
	}
}

// 인터럽트 서비스 루틴
SIGNAL(INT4_vect)
{
	
	cli();			// 전체 인터럽트 금지
	TX_data = '0';  // 문자 0을 저장
	TX_flag = 1;
	sei();			// 전체인터럽트 허용
}

// 인터럽트 서비스 루틴
SIGNAL(INT5_vect)
{
	
	cli();			// 전체 인터럽트 금지
	TX_data = '1';  // 문자 1을 저장
	TX_flag = 1;
	sei();			// 전체인터럽트 허용
}

// 인터럽트 서비스 루틴
SIGNAL(INT6_vect)
{
	
	cli();			// 전체 인터럽트 금지
	TX_data = '2';  // 문자 2을 저장
	TX_flag = 1;
	sei();			// 전체인터럽트 허용
}

// 인터럽트 서비스 루틴
SIGNAL(INT7_vect)
{
	
	cli();			// 전체 인터럽트 금지
	TX_data = '3';  // 문자 3을 저장
	TX_flag = 1;
	sei();			// 전체인터럽트 허용
}