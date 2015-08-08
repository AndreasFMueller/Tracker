/*
 * timer.c -- timer implementation
 *
 * (c) 2014 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <timer.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/wdt.h>

static unsigned short	speed = 0;

static void     timer_setup() __attribute__ ((constructor));

static void	timer_setup() {
	// initialize the timer
	TCNT1 = 0;
	OCR1B = 10000;
	TCCR1 = (1 << COM1B0) | (0 << CS10);
}

void	timer_start() {
	TCCR1 = (1 << COM1B0) | (1 << CS10);
}

void	timer_stop() {
	TCCR1 = (1 << COM1B0) | (0 << CS10);
}

/**
 * \brief Change timer speed to the new speed
 *
 * This function blocks while it is changing the motor speed slowly to the
 * requested new motor speed
 */
void	timer_speed(unsigned short newspeed) {
	// XXX implemenetation missing
}

ISR(TIMER1_COMPA_vect) {
	wdt_reset();
}
