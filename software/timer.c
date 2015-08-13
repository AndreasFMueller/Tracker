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

/*
 * because we only have two 8 bit timers on this device, we need to employ
 * two different modes. The slow mode is uses a much larger prescaler value.
 */
#define MODE_FAST	1
#define MODE_SLOW	0
static unsigned char	speed_mode = MODE_SLOW;

/*
 * The Timer register TCCR1 controls the clock source and the timer mode,
 * and since we want to start/stop the timer, we will have to repeatedly
 * change the value. The parameters in the following two constants
 * select CTC.
 * The fast mode uses the prescaler 8, while the slow mode uses prescaler 
 * 1024
 */
#define TIMER_STOP	(1 << CTC1) | (0 << COM1A0) | ( 0 << CS10)
#define TIMER_FAST	(1 << CTC1) | (0 << COM1A0) | (10 << CS10)
#define TIMER_SLOW	(1 << CTC1) | (0 << COM1A0) | ( 4 << CS10)

#define	PRESCALER_FAST	8
#define PRESCALER_SLOW	512


/*
 * The following variables are used to implement ramping up and down
 * speed and reversing the direction.
 */
static unsigned short	current_speed = 0;
static unsigned char	current_direction = 0;

static unsigned short	target_speed = 0;
static unsigned char	target_direction = 0;

/**
 * \brief initialize the two timers
 */
static void     timer_setup() __attribute__ ((constructor));

/**
 * \brief Compute the value for the compare match register
 *
 * If this function returns 0, then the motor should be turned off
 */
static inline unsigned char	speed_value(unsigned short v) {
	unsigned long	t = v;
	switch (speed_mode) {
	case MODE_FAST:
		t *= PRESCALER_FAST;
		break;
	case MODE_SLOW:
		t *= PRESCALER_SLOW;
		break;
	}
	t = F_CPU / t;
	if (t > 255) {
		return 0;
	}
	return t;
}

static void	timer_setup() {
	/*
 	 * 8 bit Timer1 is used to generate the impulses for the stepper motor
	 * controller. We use the CTC (Clear Timer on compare match) to
	 * vary the frequency of the pulses. The pulses are expected on 
	 * output OC1B = PB4
 	 */
	TCNT1 = 0;
	OCR1A = 255; // slowest speed
	OCR1B = 0; // slowest possible stepping rate, 2 Hz
	GTCCR = (1 << COM1B0);
	TCCR1 = TIMER_STOP; // timer stopped

	/*
	 * Timer0 is used to generate 100Hz interrupts
	 *
 	 * to get 100 Hz, prescale the system clock of 1000000 by 64
	 * and divide it with OCR1C by 156, this gives a frequency of
	 * 1000000 / (64 * 156) = 100.16 Hz
	 */
	TCNT0 = 0;
	TCCR0A = (0 << COM0A0) | (2 << COM0B0) | (2 << WGM00);
	TCCR0B = (0 << WGM02) | (3 << CS00);
	OCR0A = 156;	// gives 100.16Hz 
	TIMSK |= _BV(OCIE0A); // enable timer interrupt
}

void	timer_start() {
	TCCR1 = (speed_mode == MODE_FAST) ? TIMER_FAST : TIMER_SLOW;
}

void	timer_stop() {
	TCCR1 = TIMER_STOP;	// timer stopped
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

#define	speed_step	(SPEED_REWIND / 100)

static void	timer_set(unsigned short speed, unsigned short direction) {
	current_direction = direction;
	current_speed = speed;

	// compute the values to be used in the timer configuration registers
	// set timer configuration
	direction_set(current_direction);
	unsigned char	speedvalue = speed_value(current_speed);
	if (speedvalue == 0) {
		OCR1A = 0;
		timer_stop();
	} else {
		OCR1A = speedvalue;
		timer_start();
	}
}

/*
 * Timer related work to be done during the tiemr interrupt
 *
 * This is an inline function because there is no need to call it from
 * any other place than from the interrupt handler
 */
static inline void	timer_update() {
	// if the current speed is 0, then we can switch direction and mode
	// possibly the speed mode
	if (0 == current_speed) {
		// find out whether we go to the target speed in fast or
		// slow mode. In fast mode, we have to slowly speed the
		// motor up
		speed_mode = (target_speed > 255) ? MODE_FAST : MODE_SLOW;
		
		// if we are in slow mode, we go directly to the target
		// speed
		if (speed_mode == MODE_SLOW) {
			timer_set(target_direction, target_speed);
		}

		// do the first step in the direction of the target speed
		timer_set(target_direction, speed_step);
		return;
	}

	// handle slow down: we are in slow down if the target
	// direction is different from the current direction.
	if ((target_direction != current_direction)
		|| (target_speed < current_speed)) {
		// if we are in slow mode, we just stop the motor, in the
		// next iteration we will then set the new speed mode and
		// speed up the motor again
		if (speed_mode == MODE_SLOW) {
			timer_set(0, current_direction);
		} else if (current_speed > speed_step) {
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

#define DEBOUNCE_TIME	10

static unsigned char	debounce_counters[2] = { 0, 0 };

static inline unsigned char	toggle(unsigned char x) {
	return (x) ? 0 : 1;
}

/*
 * The button related work to be done during the timer interrupt
 * 
 * 
 */
static inline void	button_update() {
	if (debounce_counters[0]--) {
		// ignore state changes on the track button
	} else {
		if (track_button_state != button_state(BUTTON_TRACK)) {
			track_button_state = toggle(track_button_state);
			if (track_button_state) {
				debounce_counters[0] = DEBOUNCE_TIME;
				speed_mode = MODE_SLOW;
				timer_speed(SPEED_TRACKING, DIRECTION_FORWARD);
			}
		}
	}
	if (debounce_counters[1]--) {
		// ignore state changes on the rewind button
	} else {
		if (rewind_button_state != button_state(BUTTON_REWIND)) {
			rewind_button_state = toggle(rewind_button_state);
			if (rewind_button_state) {
				debounce_counters[1] = DEBOUNCE_TIME;
				speed_mode = MODE_FAST;
				timer_speed(SPEED_REWIND, DIRECTION_BACKWARD);
			}
		}
	}
}

/*
 * The timer interupt checks for changes in the button state and
 * also whether we have to change the speed or direction of the
 * stepper motor
 */
ISR(TIMER0_COMPA_vect) {
	button_update();
	timer_update();
	wdt_reset();
}
