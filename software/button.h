/*
 * button.h -- interface to button presses
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _button_h
#define _button_h

/**
 * \brief B0 is SW1, which is used for tracking
 */
#define BUTTON_TRACK	PINB0
/**
 * \brief B1 is SW2, which is used for rewinding
 */
#define BUTTON_REWIND	PINB1

extern unsigned char	button_states[2];

#define track_button_state button_states[0]
#define rewind_button_state button_states[1]

extern unsigned char button_state(unsigned char button);

#endif /* _button_h */
