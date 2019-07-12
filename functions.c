
/*
  Quick and dirty functions that make serial communications work.

  Note that receiveByte() blocks -- it sits and waits _forever_ for
   a byte to come in.  If you're doing anything that's more interesting,
   you'll want to implement this with interrupts.

   initUSART requires BAUDRATE to be defined in order to calculate
     the bit-rate multiplier.  9600 is a reasonable default.

  May not work with some of the older chips:
    Tiny2313, Mega8, Mega16, Mega32 have different pin macros
    If you're using these chips, see (e.g.) iom8.h for how it's done.
    These old chips don't specify UDR0 vs UDR1.
    Correspondingly, the macros will just be defined as UDR.
*/

#include <avr/io.h>

#define BV(bit)               (1 << bit)
#define sbi(x, y) x |= _BV(y)                  // set bit
#define cbi(x, y) x &= ~(_BV(y))               // clear bit
#define tbi(x, y) x ^= _BV(y)                  // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y)) //(for input) checks if the input is high (Logic 1)

#define F_CPU 16000000
#define Baud_rate 9600
#define my_ubrr (F_CPU/16/Baud_rate-1)

//UART
void init_UART()
{
	UBRR0H=(my_ubrr>>8);
	UBRR0L=my_ubrr;
	UCSR0B=(1<<TXEN0);
	UCSR0C=(1<<UCSZ01)|(1<<UCSZ00);
}
void txByte(char data) 
{
                                     /* Wait for empty transmit buffer */
  while(!is_high(UCSR0A, UDRE0));
  UDR0 = data;                                            /* send data */
}

char rxByte(void) 
{
  while(!is_high(UCSR0A, RXC0));       /* Wait for incoming data */
  return UDR0;                                /* return register value */
}

//Printing
/* Here are a bunch of useful printing commands */

void printString(const char myString[]) 
{
  char i = 0;
  while (myString[i]) {
    txByte(myString[i]);
    i++;
  }
}

void readString(char myString[], uint8_t maxLength) 
{
  char response;
  char i,exit=0;
  i = 0;
  while ((i < (maxLength - 1))&&(!exit)) {                   /* prevent over-runs */
    response = rxByte();
    if (response == '\r') {                     /* enter marks the end */
      exit=1;;
    }
    else {
      myString[i] = response;                       /* add in a letter */
      i++;
    }
  }
  myString[i] = 0;                          /* terminal NULL character */
}

void printByte(char byte) 
{
  /* Converts a byte to a string of decimal text, sends it */
  txByte('0' + (byte / 100));                        /* Hundreds */
  txByte('0' + ((byte / 10) % 10));                      /* Tens */
  txByte('0' + (byte % 10));                             /* Ones */
}

char numtoASCII(int num) 
{
    return ('0' + (char)num);
}

char getNumber(void) {
  // Gets a numerical 0-255 from the serial port.
  // Converts from string to number.
  char hundreds = '0';
  char tens = '0';
  char ones = '0';
  char thisChar = '0';
  do {                                                   /* shift over */
    hundreds = tens;
    tens = ones;
    ones = thisChar;
    thisChar = rxByte();                   /* get a new character */
  } while (thisChar != '\r');                     /* until type return */
  return (100 * (hundreds - '0') + 10 * (tens - '0') + ones - '0');
}



//TIMER0
void init_Timer0()
{
	TCCR0A=0;//normal mode((WGM0[1]=1 WGM0[0]=0 for CTC (clear timer on compare) ))
	TIMSK0=(1<<OCIE0A)|(1<<OCIE0B)|(1<<TOIE0);//enable interrupts on comaprison with output compare A,B and on overflow respectively
	OCR0A=0;
	OCR0B=0;
}
void start_Timer0()
{
	TCCR0B=(1<<CS01);//256 prescaler//start timer
}

void init_ADC()
{
	ADMUX=(1<<REFS0)|(1<< MUX2)|(1<< MUX0);//reference selection(AVCC at AREF pin) and input channel selection
	ADCSRA=(1<<ADEN)|(1<< ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//enable ADC ,enable interrupts on completing conversion ,set prescaler
	DIDR0=(1<<ADC5D);//DISABLE DIGITAL INPUTS
}
void start_adc()
{
	ADCSRA|=(1<<ADSC);//start conversion
}

//PCINT interrupt
void init_PCINT(void)
{
	PCICR = (1<<PCIE0);//enable interrupts on PCINT 0-7
	PCMSK0 = (1<<PCINT4);//Accept interrupts on PCINT4
}

//INT interrupt
void init_INT(void)
{
	EIMSK = (1<<INT1)|(1<<INT0);//accept interrupt from INT1 INT0
}

