#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000
#define Baud_rate 9600
#define my_ubrr (F_CPU/16/Baud_rate-1)

void int_pins();
void int_timer(void);
void int_adc(void);
void start_adc(void);
void start_timer(void);
void int_serial(void);
void tx_serial();
int main()
{
	int_pins();
	int_timer();
	int_adc();
	int_serial();
	sei();//enable interrupts
	start_adc();//start conversion
	start_timer();//START TIMER
	while(1);
}
void int_pins()
{
	DDRB=(1<<PB5);
}
void int_timer()
{
	TCCR0A=0;
	TIMSK0=(1<<OCIE0A)|(1<<TOIE0);
	OCR0A=0;
}
void start_timer()
{
	TCCR0B=(1<<CS01);//256 prescaler//start timer
}
ISR(TIMER0_COMPA_vect)
{
	PORTB&=~(1<<PB5);
}
ISR(TIMER0_OVF_vect)
{
	PORTB|=(1<<PB5);
}
void int_adc()
{
	ADMUX=(1<<REFS0)|(1<< MUX2)|(1<< MUX0);
	ADCSRA=(1<<ADEN)|(1<< ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	DIDR0=(1<<ADC5D);
}
void start_adc()
{
	ADCSRA|=(1<<ADSC);
}
ISR(ADC_vect)
{
	OCR0A=(ADC>>2);
	tx_serial();
	start_adc();
}
void int_serial()
{
	UBRR0H=(my_ubrr>>8);
	UBRR0L=my_ubrr;
	UCSR0B=(1<<TXEN0);
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
}
void tx_serial()
{
	UDR0=((ADC>>8))+48;
}


