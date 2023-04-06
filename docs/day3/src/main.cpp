#include<avr/io.h>
#include<util/delay.h>

unsigned char buf[4];

void display(int i);
void displayDigit();

int main(void){
	DDRD=0xFF;
	PORTD=0b00000000;
	DDRB=0x0F;
	PORTB=0x00;

	int i=0;
	while(1){
		i++;
		display(i);
		for(int d=0;d<100;d++){
			displayDigit();
			_delay_ms(1);
		}
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

void displayDigit(){
	static int sw=0;
	sw=(sw+1)%4;
	PORTB=1<<sw;
	PORTD=buf[sw];
}
