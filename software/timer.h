/*
 * timer.h -- timer configuration
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _timer_h
#define _timer_h

#include <avr/pgmspace.h>

/**
 * \brief EEPROM memory location for the tracking speed
 *
 * The tracking speed below is given in Hz. See the README for how we arrived
 * at these values.
 */
extern const unsigned short	tracking_speed PROGMEM;

/**
 * \brief The tracking speed value
 *
 * The correct value for this variable is computed in timer.c, and must
 * be written to flash. 
 */
#define	SPEED_TRACKING	tracking_speed

/**
 * \brief Maximum speed of the motor, 400Hz at 16th stepping
 */
#define SPEED_REWIND	6400

/**
 * \brief clock frequence of the processor, 1MHz
 */
#define F_CPU	1000000L

extern void	timer_start();
extern void	timer_stop();
extern void	timer_speed(unsigned short speed, unsigned char direction);

#endif /* _timer_h */
