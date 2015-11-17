/*
 * direction.h -- interface to the direction pin on the stepper driver
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _direction_h
#define _direction_h

/**
 * \brief Direction constant for forward motion
 */
#define DIRECTION_FORWARD	1

/**
 * \brief Direction constant for backward motion
 */
#define DIRECTION_BACKWARD	0

/**
 * \brief Default direction is forward
 *
 * This may be changed on the southern hemisphere
 */
#define DEFAULT_DIRECTION	DIRECTION_FORWARD

extern void	direction_default(unsigned char direction);
extern void	direction_set(unsigned char direction);

#endif /* _direction_h */

