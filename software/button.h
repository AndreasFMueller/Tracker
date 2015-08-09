/*
 * button.h -- interface to button presses
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _button_h
#define _button_h

#define BUTTON_TRACK	PINB0
#define BUTTON_REWIND	PINB1

extern unsigned char   track_button_state;
extern unsigned char   rewind_button_state;

extern unsigned char button_state(unsigned char button);

#endif /* _button_h */
