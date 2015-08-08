/*
 * timer.h -- timer configuration
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _timer_h
#define _timer_h

#define	SPEED_TRACKING	47
#define SPEED_REWIND	1291

#define F_CPU	1000000

extern void	timer_start();
extern void	timer_stop();
extern void	timer_speed(unsigned short speed, unsigned char direction);

#endif /* _timer_h */
