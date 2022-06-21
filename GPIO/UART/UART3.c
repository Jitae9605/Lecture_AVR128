/*
 * UART3.c
 *
 * Created: 2022-06-21 오전 11:23:04
 *  Author: PKNU
 */ 
// UART로 PC와 데이터 주고받기

#define F_CPU 7372800UL
#include <avr/io.h>

void putch(unsigned char data)
{
	while(!(UCSR0A & (1 << UDRE0))); // 전송준비상태 까지 대기
	UDR0 = data;					 // 데이터를 UDR0 에 쓰면 전송
	UCSR0A |= (1 << UDRE0);
}

unsigned char getch()
{
	unsigned char data;
	while (!(UCSR0A & (1 << RXC0))); // 데이터 받을때까지 대기
	data = UDR0;
	UCSR0A |= (1 << RXC0);
	return data;
	
}

int main()
{
	unsigned char text[] = 
	"\r\n Welcome! edgeiLAB\r\n USART 0 Test Program.\r\n";
	
	unsigned char echo[] = "ECHO >> ";
	unsigned char i = 0;
	
	// Rx(입력,0) / TX(출력,1)
	DDRE = 0xfe;	
	
	// 플래그 초기화
	UCSR0A = 0x00;
	
	// RX,TX enable
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	
	// 비동기 방식, no parity bit, 1 stop bit
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// 115200 bps
	UBRR0H = 0x00;
	UBRR0L = 0x03;
	
	while(text[i] != '\0')
	{
		// 문자열의 끝인 '\0'가 나올때까지 한 문자씩 출력
		putch(text[i++]);
	}
	
	// 카운터 변수 초기화
	i = 0;
	while(echo[i] != '\0')
	{
		putch(echo[i++]);
	}
	while(1)
	{
		// 입력받은 문자를 다시 putch로 문자열 출력
		putch(getch());
	}
	
}