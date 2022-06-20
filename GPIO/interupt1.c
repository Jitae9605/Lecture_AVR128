/*
 * interupt1.c
 *
 * Created: 2022-06-20 오전 10:01:47
 *  Author: PKNU
 */ 
#define F_CPU 7432800UL
#include <avr/io.h>
#include <util/delay.h>

int main()
{
	// 포트 입출력 설정
	DDRD = 0x00;
	DDRA = 0xff;
	
	// INTO 활성화
	EIMSK = 0x01; 
	
	// EICRA = 0x03; // 0000 0000
	EICRA = (1 << ISC01) | (1 << ISC00) ; // 위의 (EICRA = 0x03)와 완전히 같은 것! 표현만 다른 것이다.
	// 이 방법은 한눈에 어떤 비트를 활성화 시켰는지 알수 있어 매우 좋은 코드다!
	
	
	while(1)
	{
		
	}
	
	return 0;
}