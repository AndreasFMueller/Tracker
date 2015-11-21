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
#include <math.h>

/**
 * \brief constant to indicate fast speed used during rewind
 */
#define MODE_FAST	1

/**
 * \brief constant to indicate slow speed used during tracking
 */
#define MODE_SLOW	0

/*
 * \brief current speed mode
 *
 * Because we only have two 8 bit timers on this device, we need to employ
 * two different mode. The slow mode is uses a much larger prescaler value.
 */
static unsigned char	speed_mode = MODE_SLOW;

/*
 * The Timer register TCCR1 controls the clock source and the timer mode,
 * and since we want to be able to start/stop the timer, we will have to
 * repeatedly change the value. The parameters in the following two constants
 * select CTC.
 * The fast mode uses the prescaler 1, while the slow mode uses prescaler 
 * 32. Note that the PRESCALER_FAST and PRESCALER_SLOW constants below
 * use values one power higher, because the number of compare matches per
 * second is the number of state changes per second, and that is twice
 * the number of pulses that we want to send to the Pololu driver.
 */
#define TIMER_STOP	(1 << CTC1) | (0 << COM1A0) | ( 0 << CS10)
#define TIMER_FAST	(1 << CTC1) | (0 << COM1A0) | ( 1 << CS10)
#define TIMER_SLOW	(1 << CTC1) | (0 << COM1A0) | ( 6 << CS10)

#define	PRESCALER_FAST	2
#define PRESCALER_SLOW	64

#define	MICROSTEPS	16
#define	STEPS_PER_TURN	200

#define	BARN_DOOR_RADIUS	228
#define	BARN_DOOR_INCREMENT	0.7

#define	ANGLE_PER_TURN		(BARN_DOOR_INCREMENT / BARN_DOOR_RADIUS)
#define	ANGLE_PER_SECOND	(2 * M_PI / 86164)
#define	TURNS_PER_SECOND	(ANGLE_PER_SECOND / ANGLE_PER_TURN)
#define STEPS_PER_SECOND	(STEPS_PER_TURN * TURNS_PER_SECOND)
#define	MICROSTEPS_PER_SECOND	(MICROSTEPS * STEPS_PER_SECOND)
#define	COUNTS_PER_SECOND	(F_CPU / PRESCALER_SLOW)
#define	COUNTS_PER_MICROSTEP	(COUNTS_PER_SECOND / MICROSTEPS_PER_SECOND)

volatile unsigned short	tracking_speed = MICROSTEPS_PER_SECOND;

#define TRACKING_MODE	0
#define REWIND_MODE	1
#define STOP_MODE	2
/**
 * \brief 
 */
static volatile unsigned char	mode = 0;

/*
 * The following variables are used to implement ramping up and down
 * speed and reversing the direction.
 */
static volatile unsigned short	current_speed = 0;
static volatile unsigned char	current_direction = 0;

static volatile unsigned short	target_speed = 0;
static volatile unsigned char	target_direction = 0;

static volatile unsigned char	slowdown = 0;

/**
 * \brief initialize the two timers
 *
 * Timer0 is used to generate 100Hz interrupts. When ramping up or down
 * in the fast speed mode, the timer value is changed during the timer
 * interrupt handler.
 *
 * Timer1 is used with the PWM to generate the clock output to the STEP
 * input of the Pololu driver.
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
	OCR1C = 255; // slowest speed
	OCR1B = 0; // 
	GTCCR = (1 << COM1B0); // toggle OC1B each time the timer is reset
			       // to zero (CTC)
	TCCR1 = TIMER_STOP; // timer stopped

	/*
	 * Timer0 is used to generate 100Hz interrupts
	 *
 	 * to get 100 Hz, prescale the system clock of 1000000 by 64
	 * and divide it with OCR0A by 156, this gives a frequency of
	 * 1000000 / (64 * 156) = 100.16 Hz
	 */
	TCNT0 = 0;
	TCCR0A = (0 << COM0A0) | (0 << COM0B0) | (2 << WGM00);
	TCCR0B = (0 << WGM02) | (3 << CS00);
	OCR0A = 156;	// gives 100.16Hz matches
	TIMSK |= _BV(OCIE0A); // enable timer interrupt

	/*
	 * turn B4 into an output
	 */
	PORTB |= _BV(PORTB4);   // high
        DDRB |= _BV(DDB4);	// ensure port B4 is an output
}

/**
 * \brief Start the timer
 *
 * depending on the current speed mode, the appropriate timer configuration
 * value is loaded into the Timer 1 configuration register.
 */
void	timer_start() {
	TCCR1 = (speed_mode == MODE_FAST) ? TIMER_FAST : TIMER_SLOW;
}

/**
 * \brief Stop the timer
 *
 * The TIMER_STOP timer configuration value stops the timer
 */ 
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
		OCR1C = 0;
		timer_stop();
	} else {
		OCR1C = speedvalue + 1;
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
			timer_set(target_speed, target_direction);
			return;
		}

		// do the first step in the direction of the target speed
		timer_set(speed_step, target_direction);
		return;
	}

	// handle slow down: we are in slow down if the target
	// direction is different from the current direction.
	if (slowdown) {
		// if we are in slow mode, we just stop the motor,
		// in the next iteration we will then set the new
		// speed mode and speed up the motor again
		if (speed_mode == MODE_SLOW) {
			timer_set(0, current_direction);
			slowdown = 0;
		} else if (current_speed > speed_step) {
			timer_set(current_speed - speed_step,
				current_direction);
		} else {
			timer_set(0, current_direction);
			slowdown = 0;
		}
		return;
	}

	// if the current speed is the same as the target speed, then there
	// is no need for an update
	if ((current_speed == target_speed)
		&& (current_direction == target_direction)) {
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

#define DEBOUNCE_TIME	2

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
	if (debounce_counters[0]) {
		--debounce_counters[0];
		// ignore state changes on the track button
	} else {
		unsigned char	s = button_state(BUTTON_TRACK);
		if (track_button_state != s) {
			debounce_counters[0] = DEBOUNCE_TIME;
			track_button_state = s;
			if (mode == TRACKING_MODE) {
				return;
			}
			if (track_button_state) {
				speed_mode = MODE_SLOW;
				timer_speed(SPEED_TRACKING, DIRECTION_FORWARD);
				slowdown = 1;
				mode = TRACKING_MODE;
			}
		}
	}
	if (debounce_counters[1]) {
		--debounce_counters[1];
		// ignore state changes on the rewind button
	} else {
		unsigned char	s = button_state(BUTTON_REWIND);
		if (rewind_button_state != s) {
			debounce_counters[1] = DEBOUNCE_TIME;
			rewind_button_state = s;
			if (mode == REWIND_MODE) {
				return;
			}
			if (rewind_button_state) {
				speed_mode = MODE_FAST;
				timer_speed(SPEED_REWIND, DIRECTION_BACKWARD);
				slowdown = 1;
				mode = REWIND_MODE;
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
