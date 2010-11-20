#ifndef	TIMER_H
#define	TIMER_H


typedef void (*timer_cb) (unsigned char);

enum source
{
	INTERNAL = 0,
	EXTERNAL
};

enum prescale
{
	STOP_TIMER = 0,
	PR_1,
	PR_8,
	PR_32,
	PR_64,
	PR_128,
	PR_256,
	PR_1024


};

enum interrupt_mode
{
	OVERFLOW = 0,
	CTC

};

void init_timer2(unsigned char, unsigned char,unsigned char);
void enable_timer_2_interrupt(unsigned char,timer_cb);
void disable_timer_2_interrupt(unsigned char);
void disable_timer_2_oc(void);
void enable_timer_2_oc(unsigned char);

#endif
