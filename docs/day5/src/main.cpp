#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <math.h>

unsigned char buf[4];

void display(double d);

int main(void) {
	DDRD = 0xFF;
	PORTD = 0b00000000;
	DDRB = 0x0F;
	PORTB = 0x00;

	ADCSRA=0b10000100;
	ADMUX=0b00000000;
	_delay_ms(5);

	//timer0 制御レジスタA
	TCCR0A = 0b00000010;
	//timer0 制御レジスタB
	TCCR0B = 0b00000101;
	//timer0 割り込み設定
	TIMSK0 = 0b00000010;
	OCR0A = 4;
    
    // AD変換の設定
    ADMUX=0b00000000;
    ADCSRA=0b10000100;
    _delay_ms(5);

	//割り込みを許可
	sei();
	int i = 5000;
    volatile double temp=0.0;
	while (1) {
        
		ADCSRA|=0b01000000; //AD変換スタート
        while(!(ADCSRA&0b01000000)){}//変換中...

		double adc=(double)ADC;

        temp=1020992.5/10.0/(298.1*log(adc/(1023.0-adc))+3425.0);//温度に変換

		display(temp);
		_delay_ms(100);
	}
	return 0;
}

void display(double f) {
	const unsigned char digit[] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
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
	if(f>0){
		int d=(int)log10(f)+1; // 整数部分の桁数

		if(d<0|d>4){ //正の実数で表示可能なのは0.001~9999.9...の範囲
			return;
		}
		int i=(int)(f*(pow(10,4-d)));

		i = i % 10000;
		buf[3] = digit[i % 10];
		buf[2] = digit[(i % 100) / 10];
		buf[1] = digit[(i % 1000) / 100];
		buf[0] = digit[i / 1000];

		buf[d-1]|=0x80; // 小数点を加える
	}else if(f<0){
		int d=(int)log10(-f)+1; // 整数部分の桁数

		if(d<0|d>3){ //負の実数で表示可能なのは-999~-0.01...の範囲
			return;
		}
		int i=(int)(f*(pow(10,3-d)));

		i = i % 1000;
		buf[3] = digit[i % 10];
		buf[2] = digit[(i % 100) / 10];
		buf[1] = digit[(i % 1000) / 100];
		buf[0] = 0x40;

		buf[d-1]|=0x80; // 点を加える
	}else{
	}
}


ISR(TIMER0_COMPA_vect) {
	static int sw = 0;
	sw = (sw + 1) % 4;
	PORTB = 1 << sw;
	PORTD = buf[sw];
}