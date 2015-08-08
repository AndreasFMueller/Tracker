/*
 * direction.h -- interface to the direction pin on the stepper driver
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _direction_h
#define _direction_h

#define DIRECTION_FORWARD
#define DIRECTION_BACKWARD

extern void	direction_southern(unsigned char direction);
extern void	direction_set(unsigned char direction);

#endif /* _direction_h */

