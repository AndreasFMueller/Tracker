/*
 * button.h -- interface to button presses
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _button_h
#define _button_h

#define BUTTON_TRACK	PB0
#define BUTTON_REWIND	PB1

extern unsigned char button_state(unsigned char button);

#endif /* _button_h */
