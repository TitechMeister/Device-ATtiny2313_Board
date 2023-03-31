#include<avr/io.h>
#include<util/delay.h>

int main(void){
	DDRD=0b00000001;
	PORTD=0b00000000;
	while(1){
		PORTD=0b00000001;
		_delay_ms(1000);
		PORTD=0b00000000;
		_delay_ms(1000);
	}
	return 0;
}
