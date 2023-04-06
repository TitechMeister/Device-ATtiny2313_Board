#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>

unsigned char buf[4];

void display(int i);

int main(void){
	DDRD=0xFF;
	PORTD=0b00000000;
	DDRB=0x0F;
	PORTB=0x00;

	//timer0 制御レジスタA
	TCCR0A = 0b00000010;
	//timer0 制御レジスタB
	TCCR0B = 0b00000101;
	//timer0 割り込み設定
	TIMSK0 = 0b00000010;
	OCR0A=4;
	//割り込みを許可
	sei();
	int i=0;
	while(1){
		i--;
        if(i<0){
            i=9999;
        }
		display(i);
		_delay_ms(1000);
	}
	return 0;
}

void display(int i){
	const unsigned char digit[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};
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
	i=i%10000;
	buf[3]=digit[i%10];
	buf[2]=digit[(i%100)/10];
	buf[1]=digit[(i%1000)/100];
	buf[0]=digit[i/1000];
}

ISR(TIMER0_COMPA_vect){
	static int sw=0;
	sw=(sw+1)%4;
	PORTB=1<<sw;
	PORTD=buf[sw];
}