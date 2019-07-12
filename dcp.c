#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000			//CPU frequency
#define BAUD 9600			//BAUD rate
#define MYUBRR ((F_CPU/16/BAUD)-1)

char data;

double dc=10;//duty cycle

int main()
{
	DDRB = (1<< PB5);
	
	PORTB = (1 << PB5);
	
	OCR0A=(dc/100)*255;
	OCR0B=255;
	
	TIMSK0 = (1 << OCIE0A)|(1 << OCIE0B);//when to generate interrupt
	sei();

	//prescaler 1024
	TCCR0B = (1 << CS00)|(1<<CS01);
	
	
	
	UBRR0H = (MYUBRR >> 8);						//higher ubbr bits
	UBRR0L = (MYUBRR) ;						//lower ubbr bits
	UCSR0B = (1 << RXEN0) | (1 << RXCIE0);				//enable transmission
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);				// Set frame: 8data, 1 stp
	
	sei();
	while(1)
	{

	}
}
ISR(USART_RX_vect)//A USART receive complete interrupt will be generated only if the RXCIE0 bit is written to one, the global interrupt flag in SREG is written to one and the RXC0 bit in UCSR0A is set.
{
		cli();
		//while(!((UCSR0A)& (1 << RXC0)));//exit this loop if transmission complete(not required in interrupt mode)
		data=UDR0;			//TX-RX data register
		if(data=='0') dc=0;
		if (data=='1') dc=20;
		if (data=='2') dc=40;
		if (data=='3') dc=60;
		if (data=='4') dc=80;
		if (data=='5') dc=100;
		OCR0A=(dc/100)*255;
		//_delay_ms(1000);
		sei();
}

ISR(TIMER0_COMPA_vect)//ISR when Timer0 value equals that of OCR0A
{
	PORTB &= ~(1 << PB5);
}
ISR(TIMER0_COMPB_vect)//ISR when Timer0 value equals that of OCR0B
{
	PORTB |= (1 << PB5);
}
