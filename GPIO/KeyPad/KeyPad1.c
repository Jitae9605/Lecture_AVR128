/*
 * KeyPad1.c
 *
 * Created: 2022-06-23 오후 4:03:12
 *  Author: PKNU
 */ 
// 키패드 이용방법
// 키패드는 각각 이 핀에 연결된게 아니라 각기 다른 2개의 핀쌍에 스위치가 연결되어 있는꼴
// -> 스위치가 눌렸을때 연결된 핀의 조합으로 어느스위치가 눌린건지 판단

#define F_CPU 7372800UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char keyScan();

int main()
{
	// * 스위치 원리
	// 끊어져있던게 연결되는 것 => A핀으로 1을 출력했을때 스위치가 눌러져있었다면 1이 입력되어 들어왔을것 임을 이용해 판단
	// 단, 1을 판별하는건 오류가 많아 0을 판단한다.
	
	// *현재 연결상황
	// A	: 0    1    2    3    4    5    6    7
	// pad  : c0   c1   c2   r0   r1   r2   r3   없음
	
	// 포트A의 0 ~ 2을 출력(Row), 3 ~ 6을 입력(Colum)으로 설정
	
	unsigned char key;
	DDRA = 0xF8;  // 1111 1000
	DDRC = 0xFF;
	
	// 포트A에 들어오는 값을 판별 하기 위해 신호의 변화를 일부러 주는것
	// 0을 시그널로서 잡는다.
	
	
	while (1)
	{
		key = keyScan();
		if(key == '1') PORTC = 0x01;
	}
	return 0;
}

unsigned char keyScan()
{
	PORTA = 0xF7;							// 1111 0111  - 출력
	if(PINA == 0xFE) return '1';			// 1111 1110  - 입력받은 결과
	else if(PINA == 0xFD) return '2';		// 1111 1101  - 입력받은 결과
	else if(PINA == 0xFB) return '3';		// 1111 1011  - 입력받은 결과
	_delay_ms(10);
	
	PORTA = 0xEF;							// 1110 1111  - 출력
	if(PINA == 0xFE) return '4';			// 1111 1110  - 입력받은 결과
	else if(PINA == 0xFD) return '5';		// 1111 1101  - 입력받은 결과
	else if(PINA == 0xFB) return '6';		// 1111 1011  - 입력받은 결과
	_delay_ms(10);
	
	PORTA = 0xDF;							// 1101 1111  - 출력
	if(PINA == 0xFE) return '7';			// 1111 1110  - 입력받은 결과
	else if(PINA == 0xFD) return '8';		// 1111 1101  - 입력받은 결과
	else if(PINA == 0xFB) return '9';		// 1111 1011  - 입력받은 결과
	_delay_ms(10);
	
	PORTA = 0xBF;							// 1011 1111  - 출력
	if(PINA == 0xFE) return '*';			// 1111 1110  - 입력받은 결과
	else if(PINA == 0xFD) return '0';		// 1111 1101  - 입력받은 결과
	else if(PINA == 0xFB) return '#';		// 1111 1011  - 입력받은 결과
	_delay_ms(10);
}