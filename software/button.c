/*
 * button.c -- button interface
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <button.h>
#include <avr/io.h>

unsigned char	button_states[2] = { 0 /* track */, 0 /* rewind */ };

/*
 * button_setup is supposed to be a constructor to be called automatically
 * when the tracker initializes
 */
unsigned char	button_state(unsigned char button);
void	button_setup(void) __attribute__ ((constructor));

/**
 * \brief set up the buttons
 *
 * We have to buttons called SW1 and SW2 on the schematic. SW1 corresponds
 * to the lower button, connected to B0, SW2 is the upper button, connected
 * to B1. This method initializes the buttons as inputs.
 */
void	button_setup(void) {
	PORTB |= _BV(PORTB0) | _BV(PORTB1);
	DDRB &= ~(_BV(DDB0) | _BV(DDB1));

	/* initialize the button state variables with the current value
	   read from the button inputs */
	track_button_state = button_state(BUTTON_TRACK);
	rewind_button_state = button_state(BUTTON_REWIND);
}

/**
 * \brief Query the button state
 *
 * Read the button value from input register PINB. Buttons give high
 * values if not pressed, this functions turns them into defined values
 * 0 and 1.
 */
unsigned char	button_state(unsigned char button) {
	return (PINB & _BV(button)) ? 0 : 1;
	return 0;
}
