#include <avr/io.h>
#include <avr/interrupt.h>
#include "int_ext.h"

void (* function_INT0) (void) = 0;
void (* function_INT1) (void) = 0;

void configure_INT0(unsigned char etat, void (*function)(void) )
{
	function_INT0 = function;

	switch(etat)
	{
		case RISING_EDGE: MCUCR |= (1<<ISC00) + (1<<ISC01); GICR |= (1<<INT0); break;
		case FALLING_EDGE: MCUCR |= (1<<ISC01); MCUCR &= ~(1<<ISC00); GICR |= (1<<INT0); break;
		case ANY_CHANGE: MCUCR |= (1<<ISC00); MCUCR &= ~(1<<ISC01); GICR |= (1<<INT0); break;
		case OFF: GICR &= ~(1<<INT0); break;

		default: GICR &= ~(1<<INT0); break;
	}

	DDRD &= ~(1<<2);

	sei();
}

void configure_INT1(unsigned char etat, void (*function)(void) )
{
	function_INT1 = function;

	switch(etat)
	{
		case RISING_EDGE: MCUCR |= (1<<ISC10) + (1<<ISC11); GICR |= (1<<INT1); break;
		case FALLING_EDGE: MCUCR |= (1<<ISC11); MCUCR &= ~(1<<ISC10); GICR |= (1<<INT1); break;
		case ANY_CHANGE: MCUCR |= (1<<ISC10); MCUCR &= ~(1<<ISC11); GICR |= (1<<INT1); break;
		case OFF: GICR &= ~(1<<INT1); break;

		default: GICR &= ~(1<<INT1); break;
	}

	DDRD &= ~(1<<3);

	sei();
}

ISR(INT0_vect)
{
	(*function_INT0)();
}

ISR(INT1_vect)
{
	(*function_INT1)();
}
