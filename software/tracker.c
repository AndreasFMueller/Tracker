/*
 * guiderport.c -- main function of the guiderport firmware
 *
 * (c) 2015 Prof Dr Andreas Müller, Hochschule Rapperswil
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */
#include <avr/wdt.h>
#include <led.h>
#include <timer.h>
#include <button.h>
#include <direction.h>
#include <util/delay.h>

int	main(int argc, char *argv[]) {
	led_on(LED1);
	led_on(LED2);
	_delay_ms(1000);
	led_off(LED1);
	led_off(LED2);
	// check whether the track button is pressed, which would mean that
	// we start up in southern hemisphere mode
	if (button_state(BUTTON_TRACK)) {
		// XXX switch to southern hemisphere mode
	}

	// initialize the timer with the initial tracking speed
	timer_start();
	timer_speed(SPEED_TRACKING);

	// guider port main function, infinite loop
	for (;;) {
	}
}

void wdt_init(void) __attribute__((naked)) __attribute__((section(".init3")));

void wdt_init(void) {
	MCUSR = 0;
	wdt_disable();
}
