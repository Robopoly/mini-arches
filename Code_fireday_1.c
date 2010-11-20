#define F_CPU 8000000UL
#define BAUD  	9600

#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include <util/setbaud.h>
#include"code_fireday_1.h"
#include"timer.h"
#include"int_ext.h"

#define VALUE2 5*128
#define VALUE1 25*128

volatile unsigned char left = 0,right = 0;
volatile static uint16_t counter_left1,counter_left2,counter_right1,counter_right2;
volatile static uint8_t flag_l, flag_r;

#define	LEFT_LASER	1
#define	RIGHT_LASER	4


void servo_left_up(void);
void servo_right_up(void);
void uartSendByte(unsigned char a);

void t2_cb(unsigned char);

void t2_cb(unsigned char arg)
{

	//uartSendByte(9);
		//PORTC|=(1<<2);
	if(left)	counter_left1++;
	if(right)	counter_right1++;

	if(PINA&RIGHT_LASER)
	{
		counter_right2++;
		//uartSendByte(counter_right2);
	}
	if(PINA&LEFT_LASER)
	{
		counter_left2++;
		//uartSendByte(counter_left2);
	}

	//static int post;
	/*if(post<100)post++;
	else
	{
		post = 0;
		PORTC^=(1<<2);
	}*/
}


void int0_cb(void)
{

	ir_blasters_down();
	uartSendByte(1);
	while((PIND&(1<<2)) == 0);
	uartSendByte(2);
		_delay_ms(1000);
	ir_blasters_rest();


	//Reset configuration
	flag_l = 0;
	flag_r = 0;
	left = 0;
	right = 0;
	counter_left1 = 0;counter_left2 = 0;
	counter_right1 = 0; counter_right2 = 0;
}

int main(void)
{
	//IO CONFIGURATIONS
	SERVO_DDR|=SERVO_MASK;
	DDRC|=(1<<2);
	DDRD&=~(1<<2);
	PORTD|=(1<<2);//Pull up
	_delay_ms(1);

	counter_left1 = 0;counter_left2 = 0;
	counter_right1 = 0; counter_right2 = 0;




	//Wait for Enable match signal
	while((PIND&(1<<2)) == 0);

	ir_blasters_down();
	_delay_ms(1000);
	ir_blasters_rest();

	configure_INT0(FALLING_EDGE,int0_cb);

	//while(((PINA&1) == 0) && (((PINA&(1<<2)) == 0)));

	//Enable TIMER2
	init_timer2(PR_128,EXTERNAL,1);
	enable_timer_2_interrupt(CTC,t2_cb);


	while(1)
	{
		if(PINA&LEFT_LASER)	left = 1;
		if(PINA&RIGHT_LASER)	right = 1;

		uartSendByte(flag_l);

		if((counter_left1 >= VALUE1 || counter_left2 >=VALUE2) && (flag_l == 0))
		{
			//uartSendByte(5);
			servo_left_up();
			flag_l = 1;
		}
		
		if((counter_right1>=VALUE1 || counter_right2 >=VALUE2)  && (flag_r == 0))
		{
			//uartSendByte(6);
			servo_right_up();
			flag_r = 1;
		}

	}





return 0;
}




#define OCR_OFFSET 383
void set_pwm(uint16_t left, uint16_t right)
{
    // 8 bit fast pwm
    TCNT1 = 0;
    TCCR1A = (0b11 << WGM10) | // WGM11:10
                (0b10 << COM1A0) |
                //(0b11 << FOC1B) | // FOC1A:B
                (0b10 << COM1B0);
    TCCR1B = (0b01 << WGM12) | // WGM13:12
                (0b100 << CS10); // prescaler 256
    OCR1A = OCR_OFFSET+right;
    OCR1B = OCR_OFFSET+left;
    TIMSK |= _BV(TOIE1);
    sei();
}

void set_pwm_left(uint16_t left)
{
    // 8 bit fast pwm
    TCNT1 = 0;
    TCCR1A = (0b11 << WGM10) | // WGM11:10
                (0b10 << COM1A0) |
                //(0b11 << FOC1B) | // FOC1A:B
                (0b10 << COM1B0);
    TCCR1B = (0b01 << WGM12) | // WGM13:12
                (0b100 << CS10); // prescaler 256
    //OCR1A = OCR_OFFSET+right;
    OCR1B = OCR_OFFSET+left;
    TIMSK |= _BV(TOIE1);
    sei();
}
void set_pwm_right(uint16_t right)
{
    // 8 bit fast pwm
    TCNT1 = 0;
    TCCR1A = (0b11 << WGM10) | // WGM11:10
                (0b10 << COM1A0) |
                //(0b11 << FOC1B) | // FOC1A:B
                (0b10 << COM1B0);
    TCCR1B = (0b01 << WGM12) | // WGM13:12
                (0b100 << CS10); // prescaler 256
    OCR1A = OCR_OFFSET+right;
    //OCR1B = OCR_OFFSET+left;
    TIMSK |= _BV(TOIE1);
    sei();
}

void servo_left_up(void)
{
	set_pwm_left(40);
}

void servo_right_up(void)
{
	set_pwm_right(65);
}

void disable_pwm(void)
{
    TCCR1A = 0;
    TCCR1B = 0;
    OCR1A = 0;
    OCR1B = 0;
    TIMSK &= ~_BV(TOIE1);
    //TIMSK&=~(1<<OCIE1A);
	//TIMSK&=~(1<<OCIE1B);
	//TIMSK = 0;
}

ISR(TIMER1_OVF_vect)
{
    TCNT1 = OCR_OFFSET;
}

// ---------------------------
// SERVO Functions
// ---------------------------

void ir_blasters_down(void)
{
    set_pwm(70, 32);

    return;
}

// ----------------------------

void ir_blasters_up(void)
{
    set_pwm(40, 65);
    return;
}

// ----------------------------

void ir_blasters_rest(void)
{
    disable_pwm();
    SERVO_PORT &= ~SERVO_MASK;

    return;
}



static unsigned char uartStatus = 1;
void uartSendByte(unsigned char a)
{
	if(uartStatus)
	{
		UBRRH = UBRRH_VALUE;
		UBRRL = UBRRL_VALUE;
		#if USE_2X
		UCSRA |= (1 << U2X);
		#else
		UCSRA &= ~(1 << U2X);
		#endif
		UCSRB |= 0x18;
		uartStatus = 0;
	}

	while((UCSRA & (1<<UDRE))==0);
	UDR = a;
}

