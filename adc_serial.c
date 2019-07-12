#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 16000000
#define Baud_rate 9600
#define my_ubrr (F_CPU/16/Baud_rate-1)

unsigned char array[10];

void int_adc(void);
void start_adc(void);
void start_timer(void);
void int_serial(void);
void tx_write(unsigned int data);
void tx_serial(char data);
char tx_complete(void);
int main()
{
	int_adc();
	int_serial();
	sei();//enable interrupts
	start_adc();//start conversion
	while(1);
}

void int_adc()
{
	ADMUX=(1<<REFS0)|(1<< MUX2)|(1<< MUX0);
	ADCSRA=(1<<ADEN)|(1<< ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	DIDR0=0;
}
void start_adc()
{
	ADCSRA|=(1<<ADSC);
}
ISR(ADC_vect)
{
	tx_write(ADC);
	_delay_ms(1000);
	start_adc();
}

void int_serial()
{
	UBRR0H=(my_ubrr>>8);
	UBRR0L=my_ubrr;
	UCSR0B=(1<<TXEN0);
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
}
void tx_write(unsigned int data)
{
	int i=0,j;
	if (data==0)
	{
		tx_serial(48);
	}
	while(data != 0)
	{
		array[i]=(unsigned char)(data%10);
		data=data/10;
		i++;
	}
	i--;
	j=i;
	while(i>=0)
	{
		tx_serial(48+array[i]);
		i--;
	}
	tx_serial('\n');
	tx_serial('\r');
}
void tx_serial(char data)
{
	UDR0=data;
	//_delay_ms(10);
	while(!tx_complete());
}
char tx_complete(void)
{
	if (UCSR0A&(1<<TXC0))
	{
		UCSR0A|=(1<<TXC0);
		return 1;
	}
	else
	{
		return 0;
	}
}




