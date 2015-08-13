/*
 * timer.h -- timer configuration
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _timer_h
#define _timer_h

/*
 * The tracking speed below is given in Hz. See the README for how we arrived
 * at these values.
 */
#define	SPEED_TRACKING	83
#define SPEED_REWIND	6400

#define F_CPU	1000000L

extern void	timer_start();
extern void	timer_stop();
extern void	timer_speed(unsigned short speed, unsigned char direction);

#endif /* _timer_h */
