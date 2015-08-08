/*
 * led.h -- turn LEDs on or off
 *
 * (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
 */
#ifndef _led_h
#define _led_h

#define	LED1	0
#define	LED2	1

extern void	led_on(unsigned char led);
extern void	led_off(unsigned char led);

#endif /* _led_h */
