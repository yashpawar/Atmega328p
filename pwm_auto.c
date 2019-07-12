

#include <avr/io.h> // Standard include for AVR

#define sbi(x, y) x |= _BV(y)    // set bit
#define cbi(x, y) x &= ~(_BV(y)) // clear bit
#define tbi(x, y) x ^= _BV(y)    // toggle bit
#define is_high(x, y) ((x & _BV(y)) == _BV(y))

int main()
{
    DDRB = 0x20; // PORTB as OUTPUT
    PORTB = 0x00;
    
    int i, j , k,l;
    j=10;
    l=0;
    while(1) // Infinite loop
    {
        for (k=0;k<10000;k++)
        {
		for(i = 0; i < 100; i++)//variable j represents time out of 100 for which we want out lead to be on
		{
			if(i < j)
				sbi(PORTB, PB5);
			else
				cbi(PORTB, PB5);
		}	
	}
	if (l==0) j+=20;
	if (l==1) j-=20;
	if (j>100) l=1;
	if (j<0) l=0;
    }
    return 0;
}
