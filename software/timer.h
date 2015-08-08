/*
 * timer.h -- timer configuration
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _timer_h
#define _timer_h

#define	SPEED_TRACKING	47
#define SPEED_REWIND	1291

extern void	timer_start();
extern void	timer_stop();
extern void	timer_speed(unsigned short);

#endif /* _timer_h */
