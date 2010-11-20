void ir_blasters_up(void);
void ir_blasters_down(void);
void ir_blasters_rest(void);


#define SERVO_PORT PORTD
#define SERVO_DDR DDRD
#define SERVO_LEFT PD5
#define SERVO_RIGHT PD4

#define SERVO_MASK (_BV(SERVO_LEFT) | _BV(SERVO_RIGHT))
