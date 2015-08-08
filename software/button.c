/*
 * button.c -- button interface
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <button.h>

unsigned char	button_state(unsigned char button);

void	button_setup(void) __attribute__ ((constructor));

void	button_setup(void) {
	PORTB |= _BV(PORTB0) | _BV(PORTB1);
	DDRB &= ~(_BV(DDRB0) | _BV(DDRB1));
}

unsigned char	button_state(unsigned char button) {
	return (PINB & _BV(button)) ? 0 : 1;
	return 0;
}
