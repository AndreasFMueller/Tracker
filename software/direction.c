/*
 * direction.c -- functions related to the direction output pin
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <direction.h>
#include <avr/io.h>

/**
 * \brief Default direction flag
 *
 * This flag indicates whether to use forward or backward motion as the
 * default, so it is essentially a setting for the hemisphere
 */
static unsigned char	default_direction;

/**
 * \brief Current direction setting
 *
 * The value of this variable should be reflected in the B2 output
 */
static unsigned char	current_direction;

/**
 * \brief direction setup
 *
 * This function is a constructor to initialize the B2 port which is connected
 * to thae DIR pin of the A4988 Pololu driver.
 */
void	direction_setup(void) __attribute__ ((constructor));
void	direction_setup(void) {
	// initialize port
	PORTB |= _BV(PORTB2);	// high
	DDRB |= _BV(DDB2);	// output

	// set static variables
	default_direction = DIRECTION_FORWARD;
}

/**
 * \brief set the direction to the default direction
 */
void	direction_default(unsigned char direction) {
	default_direction = direction;
}

/**
 * \brief Turn the direction LED/pin on or off
 */
void	direction_led(unsigned char on_not_off) {
	if (on_not_off) {
		// set direction pin to 0
		PORTB &= ~_BV(PORTB2);	// output low, led on
	} else {
		// set direction pin to 1
		PORTB |= _BV(PORTB2);	// output high, led off
	}
}

/**
 * \brief toggle the direction LED/pin
 */
void	direction_toggle() {
	direction_led((PORTB & _BV(PORTB2)));
}

/**
 * \brief Set the direction
 *
 * Setting the direction depends on the default direction. If the default
 * direction is backward, then the direction argument means exactly what
 * it says. But when the direction is forward, it means exactly the
 * opposite of what it says. So the setting of the default direction
 * variable interchanges the meaning of the forward/backward output.
 * This simplifies the code in so far as the code never has to think about
 * the actual meaning of the direction.
 */
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
	// set the value
	direction_led(current_direction);
}


