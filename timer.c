#include<avr/io.h>
#include<avr/interrupt.h>
#include"timer.h"

timer_cb pt_timer2 = 0;

void init_timer2(unsigned char prescale, unsigned char source,unsigned char compare)
{
	TCNT2 = 0; //reset counter
	ASSR = (source<<AS2);
	TCCR2 = prescale;
	OCR2 = compare;

	if(compare != 0xFF)
		TCCR2 |=(1<<3);
	else
		TCCR2 &=~(1<<3);

}

void enable_timer_2_interrupt(unsigned char interrupt_mode, timer_cb callback)
{
	pt_timer2 = callback;

	if(interrupt_mode == CTC)
	{
		TIMSK |= (1<<OCIE2);
		//TIMSK &=~(1<<OCIE2);
	}
	else
	{
		TIMSK |= (1<<TOIE2);	
		//TIMSK &=~(1<<TOIE2);
	}
	sei();
}

//Not implemented yet!
void enable_timer_2_oc(unsigned char oc2_mode)
{
	

}

void disable_timer_2_oc(void)
{
	TCCR2&=~(1<<COM20);
	TCCR2&=~(1<<COM21);
}

void disable_timer_2_interrupt(unsigned char interrupt_mode)
{
	if(interrupt_mode == CTC)
	{
		TIMSK &= ~(1<<OCIE2);
	}
	else
	{
		TIMSK &= ~(1<<TOIE2);	
	}
}


ISR(TIMER2_OVF_vect)
{
	if(pt_timer2)
		pt_timer2(OVERFLOW);
}

ISR(TIMER2_COMP_vect)
{
	if(pt_timer2)
		pt_timer2(CTC);
}







