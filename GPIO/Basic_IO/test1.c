/*
 * test1.c
 *
 * Created: 2022-06-17 오전 11:46:07
 *  Author: PKNU
 */ 
#define F_CPU 7372800 // CPU클럭수  = 이를 기준으로 1초를 측정
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{

	// 포트의 출력을 설정
	DDRA = 0xFF;  // portA를 출력으로 설정 (이진수 = 1111 1111)
	DDRB = 0xFF;  // portB를 출력으로 설정 (이진수 = 1111 1111)
	DDRC = 0xFF;  // portC를 출력으로 설정 (이진수 = 1111 1111)
	DDRD = 0xFF;  // portD를 출력으로 설정 (이진수 = 1111 1111)
	DDRE = 0xFF;  // portE를 출력으로 설정 (이진수 = 1111 1111)
	DDRF = 0xFF;  // portF를 출력으로 설정 (이진수 = 1111 1111)
	DDRG = 0xFF;  // portG를 출력으로 설정 (이진수 = 1111 1111)
	

	// DDRA = 0x0F;  // portA의 상위 4개 비트는 입력, 하위 4개는 출력으로 설정 (이진수 = 0000 1111)
	while(1)
	{
		// PORT = 출력, PIN = 입력
		PORTA = 0xFF;	// PORTA의 각 비트에 5V출력
		PORTB = 0xFF;	// PORTB의 각 비트에 5V출력
		PORTC = 0xFF;	// PORTC의 각 비트에 5V출력
		PORTD = 0xFF;	// PORTD의 각 비트에 5V출력
		PORTE = 0xFF;	// PORTE의 각 비트에 5V출력
		PORTF = 0xFF;	// PORTF의 각 비트에 5V출력
		PORTG = 0xFF;	// PORTG의 각 비트에 5V출력
		_delay_ms(1000);
		PORTA = 0x00;	// PORTA의 각 비트에 5V출력 x
		PORTB = 0x00;	// PORTB의 각 비트에 5V출력 x
		PORTC = 0x00;	// PORTC의 각 비트에 5V출력 x
		PORTD = 0x00;	// PORTD의 각 비트에 5V출력 x
		PORTE = 0x00;	// PORTE의 각 비트에 5V출력 x
		PORTF = 0x00;	// PORTF의 각 비트에 5V출력 x
		PORTG = 0x00;	// PORTG의 각 비트에 5V출력 x
		_delay_ms(1000);
	}
	
	return 0;
}