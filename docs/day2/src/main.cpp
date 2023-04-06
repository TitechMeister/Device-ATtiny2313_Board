#include<avr/io.h>
#include <stdint.h>
#include<util/delay.h>


void display(int);

int main(void){
	PORTD=0x00;
	DDRD=0xFF;
	PORTC=0x01;
	DDRC=0x01;
	int d=0;
	while(1){
		display(d);
		_delay_ms(1000);
		d=(d+1)%10;
	}
	return 0;
}

void display(int d){
	/*ピンの接続:
	 * PORTD:
	 * 	0 -> A		
	 * 	1 -> B		    A
	 * 	2 -> C		  +---+
	 * 	3 -> D		F | G |B
	 * 	4 -> E		  +---|
	 * 	5 -> F		E |   |C
	 * 	6 -> G		  +---+ .
	 * 	7 -> DP		    D   DP
	 * */
	const unsigned char digit[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
	if(0<=d && d<=9){
		PORTD=digit[d];
	}
}
