/*
 * button.c -- button interface
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <button.h>
#include <avr/io.h>

unsigned char   track_button_state = 0;
unsigned char   rewind_button_state = 0;

unsigned char	button_state(unsigned char button);

void	button_setup(void) __attribute__ ((constructor));

void	button_setup(void) {
	PORTB |= _BV(PORTB0) | _BV(PORTB1);
	DDRB &= ~(_BV(DDB0) | _BV(DDB1));

	track_button_state = button_state(BUTTON_TRACK);
	rewind_button_state = button_state(BUTTON_REWIND);
}

unsigned char	button_state(unsigned char button) {
	return (PINB & _BV(button)) ? 0 : 1;
	return 0;
}
