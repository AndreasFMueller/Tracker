/*
 * direction.h -- interface to the direction pin on the stepper driver
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _direction_h
#define _direction_h

#define DIRECTION_FORWARD	1
#define DIRECTION_BACKWARD	0
#define DEFAULT_DIRECTION	DIRECTION_FORWARD

extern void	direction_default(unsigned char direction);
extern void	direction_set(unsigned char direction);

#endif /* _direction_h */

