/*
 * UART1.c
 *
 * Created: 2022-06-21 오전 9:39:58
 *  Author: PKNU
 */ 

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	while(1)
	{
		unsigned char text[] = "Hello! World! \r\n";
		unsigned char i = 0;
		
		// 포트 입출력 설정
		DDRE = 0xfe;	// Rx 만 입력(0), TX는 출력(1) = PE0 만 0으로 설정
		
		// USART 제어 및 상태 레지스터 A
		UCSR0A = 0x00;	// 초기 이므로 모두 0으로 설정
		
		// USART 제어 및 상태 레지스터 B
		UCSR0B = (1 << TXEN0) | (1 << RXEN0);	// Rx, TX Enable = 0001 0010
		
		// USART 제어 및 상태 레지스터 C
		UCSR0C = (1 << UCSZ01 ) | (1 << UCSZ00) ;	// 비동기방식, No Parity bit, 1 Stop bit = 0000 0110
		
		// USART baud Rate 레지스터
		// UBRRnH의 4비트와 UBRRnL의 8비트가 조합을 이룸.
		UBRR0H = 0x00;
		UBRR0L = 0x03; // 3 = 115200 bps // CPU 클럭수에 연관있으므로 데이터시트 참고할것
		
		while(text[i] != '\0')
		{
			putch(text[i++]);
		}
	}
	return 0;
}

void putch(unsigned char data)
{
	// UCSR0A 0x20 = 0010 0000  = 송신 데이터 레지스터 준비완료 인터럽트(Data Register Empty) Enable상태
	
	while(!(UCSR0A & (1 << UDRE0))); // 둘(UCSR0A,0x20)의 비트값중 하나라도 0이면 0 출력	
	UDR0 = data;
	UCSR0A |= 0x20;
	
	
}

