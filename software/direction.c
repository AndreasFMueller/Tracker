/*
 * direction.c 
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <direction.h>

static unsigned char	southern_mode;
static unsigned char	current_direction;

void	direction_setup(void) __attribute__ ((constructor));

void	direction_setup(void) {
	// XXX initialize ports
	PORTB |= _BV(PORTB2);
	DDRB |= _BV(DDRB2);

	// set static variables
	southern_mode = 0;
}

void	direction_southern(unsigned char direction) {
	southern_mode = direction;
}

void	direction_set(unsigned char direction) {
	if (southern_mode) {
		current_direction = (direction) ? 0 : 1;
	} else {
		current_direction = (direction) ? 1 : 0;
	}
	if (current_direction) {
		// set direction pin to 0
		PORTB &= ~_BV(PORTB2);
	} else {
		// set direction pin to 1
		PORTB |= _BV(PORTB2);
	}
}


