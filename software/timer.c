/*
 * timer.c -- timer implementation
 *
 * The tiny's 16 bit counter/timer is used twofold here. The compare
 * unit B is used to generated impulses for the stepper moter PB4.
 * The compare unit A is used to generate interupts for the control
 * of the buttons, and it also resets the watchdog timer.
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <timer.h>
#include <direction.h>
#include <button.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/wdt.h>

static unsigned short	current_speed = 0;
static unsigned short	target_speed = 0;
static unsigned char	current_direction = 0;
static unsigned char	target_direction = 0;

static void     timer_setup() __attribute__ ((constructor));

static void	timer_setup() {
	// initialize the timer1
	TCNT1 = 0;
	unsigned short	speed = 65535;
	OCR1B = speed; // slowest possible stepping rate, 2 Hz
	TCCR1 = (1 << COM1B0) | (0 << CS10); // timer stopped

	// initialize the timer0
	TCNT0 = 0;
	OCR0A = 156;	// gives 100.16Hz 
	TCCR0A = (0 << COM0A0) | (2 << WGM00);
	TCCR0B = (0 << WGM02) | (3 << CS00); // prescaler clk/64
	TIMSK |= _BV(OCIE1A); // enable timer interrupt
}

void	timer_start() {
	TCCR1 = (1 << COM1B0) | (4 << CS10);	// prescaler /4
}

void	timer_stop() {
	TCCR1 = (1 << COM1B0) | (0 << CS10);	// timer stopped
}

/**
 * \brief Change timer speed to the new speed
 *
 * This function blocks while it is changing the motor speed slowly to the
 * requested new motor speed
 */
void	timer_speed(unsigned short speed, unsigned char direction) {
	target_direction = direction;
	target_speed = speed;
}

#define	speed_step	0

static const unsigned long	frequency = 1000000 / 4;

static void	timer_set(unsigned short speed, unsigned short direction) {
	current_direction = direction;
	current_speed = speed;
	// compute the values to be used in the timer configuration registers
	// set timer configuration
	direction_set(current_direction);
	unsigned long	speedvalue = frequency / current_speed;
	if (speedvalue > 65535) {
		OCR1B = (unsigned short)65535;
		timer_stop();
	} else {
		unsigned short	s = speedvalue;
		OCR1B = s;
		timer_start();
	}
}

void	timer_update() {
	// if the current speed is 0, then we can switch to the direction
	// of the target speed
	if (0 == current_speed) {
		timer_set(target_direction, speed_step);
		return;
	}
	// handle slow down
	if ((target_direction != current_direction)
		|| (target_speed < current_speed)) {
		if (current_speed > speed_step) {
			timer_set(current_speed - speed_step,
				current_direction);
		} else {
			timer_set(0, current_direction);
		}
		return;
	}
	// handle speeding up
	if (current_speed < target_speed) {
		if (current_speed + speed_step < target_speed) {
			timer_set(current_speed + speed_step, target_direction);
		} else {
			timer_set(target_speed, target_direction);
		}
	}
}

static unsigned char	debounce_counter = 0;
static unsigned char	track_button_state = 0;
static unsigned char	rewind_button_state = 0;

unsigned char	toggle(unsigned char x) {
	return (x) ? 0 : 1;
}

void	button_update() {
	if (debounce_counter--) {
		return;
	}
	if (track_button_state != button_state(BUTTON_TRACK)) {
		track_button_state = toggle(track_button_state);
		if (track_button_state) {
			debounce_counter = 10;
			timer_speed(SPEED_TRACKING, DIRECTION_FORWARD);
		}
	}
	if (rewind_button_state != button_state(BUTTON_REWIND)) {
		rewind_button_state = toggle(rewind_button_state);
		if (rewind_button_state) {
			debounce_counter = 10;
			timer_speed(SPEED_REWIND, DIRECTION_BACKWARD);
		}
	}
}

ISR(TIMER0_COMPA_vect) {
	button_update();
	timer_update();
	wdt_reset();
}
