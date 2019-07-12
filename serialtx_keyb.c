#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000			//CPU frequency
#define BAUD 9600			//BAUD rate
#define MYUBRR ((F_CPU/16/BAUD)-1)

int hexkey=0x40;
int main()
{	
	DDRB=(1 << PB2);
	PORTB = (1 << PB5)|(1 << PB4)|(1 << PB3);
	UBRR0H =  (MYUBRR >> 8);		//higher ubbr bits
	UBRR0L =  (MYUBRR) ;			//lower ubbr bits
	UCSR0B = (1 << TXEN0);			//enable transmission
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame: 8data, 1 stp
	
	PCICR = (1<<PCIE0);
	PCMSK0 = (1<<PCINT5)|(1 << PCINT4)|(1 << PCINT3);
	sei();
	while(1)
	{

	}
}
				
ISR(PCINT0_vect)
{
	cli();
	while(!(PINB&(1<<PB5)))
	{
		while(!((UCSR0A)& (1 << UDRE0)));//The UDRE0 flag indicates if the transmit buffer (UDR0) is ready to receive new data.
		UDR0 = hexkey;	//TX-RX data register
		PORTB|=(1 << PB2);
		_delay_ms(1000);
		PORTB&=~(1 << PB2);			
	}
	while(!(PINB&(1<<PB4)))
	{
		hexkey++;
		if(hexkey>0x7E) hexkey=0x7E;
		PORTB|=(1 << PB2);
		_delay_ms(1000);
		PORTB&=~(1 << PB2);
	}
	while(!(PINB&(1<<PB3)))
	{
		hexkey--;
		if(hexkey<0x20) hexkey=0x20;
		PORTB|=(1 << PB2);
		_delay_ms(1000);
		PORTB&=~(1 << PB2);
	}
	sei(); 	
}


