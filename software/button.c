/*
 * button.c -- button interface
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#include <button.h>

unsigned char	button_state(unsigned char button);

void	button_setup(void) __attribute__ ((constructor));

void	button_setup(void) {
	// XXX initialize ports
}

unsigned char	button_state(unsigned char button) {
	// XXX implementation missing
	return 0;
}
