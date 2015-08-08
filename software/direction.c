/*
 * direction.c 
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <direction.h>
#include <avr/io.h>

static unsigned char	default_direction;
static unsigned char	current_direction;

void	direction_setup(void) __attribute__ ((constructor));

void	direction_setup(void) {
	// initialize port
	PORTB |= _BV(PORTB2);	// high
	DDRB |= _BV(DDB2);	// output

	// set static variables
	default_direction = DIRECTION_FORWARD;
}

void	direction_default(unsigned char direction) {
	default_direction = direction;
}

void	direction_set(unsigned char direction) {
	switch (default_direction) {
	case DIRECTION_FORWARD:
		switch (direction) {
		case DIRECTION_FORWARD:
			current_direction = DIRECTION_BACKWARD;
			break;
		case DIRECTION_BACKWARD:
			current_direction = DIRECTION_FORWARD;
			break;
		}
		break;
	case DIRECTION_BACKWARD:
		switch (direction) {
		case DIRECTION_FORWARD:
			current_direction = DIRECTION_FORWARD;
			break;
		case DIRECTION_BACKWARD:
			current_direction = DIRECTION_BACKWARD;
			break;
		}
		break;
	}
	if (current_direction) {
		// set direction pin to 0
		PORTB &= ~_BV(PORTB2);	// output low
	} else {
		// set direction pin to 1
		PORTB |= _BV(PORTB2);	// output high
	}
}


