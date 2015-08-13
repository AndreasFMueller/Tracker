/*
 * tracker.c -- main function of the tracker firmware
 *
 * (c) 2015 Prof Dr Andreas Müller, Hochschule Rapperswil
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <avr/wdt.h>
#include <timer.h>
#include <button.h>
#include <direction.h>
#include <util/delay.h>

int	main(int argc, char *argv[]) {
	_delay_ms(1000);
	// check whether a button is pressed, which would mean that
	// we start up in southern hemisphere mode
	if (button_state(BUTTON_TRACK) || button_state(BUTTON_REWIND)) {
		switch (DEFAULT_DIRECTION) {
		case DIRECTION_FORWARD:
			direction_default(DIRECTION_BACKWARD);
			break;
		case DIRECTION_BACKWARD:
			direction_default(DIRECTION_FORWARD);
			break;
		}
	}

	// initialize the timer with the initial tracking speed
	timer_start();
	timer_speed(SPEED_TRACKING, DIRECTION_FORWARD);

	// guider port main function, infinite loop
	for (;;) {
	}
}

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void) {
	MCUSR = 0;
	wdt_disable();
}
