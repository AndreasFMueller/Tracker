#
# process this file with autoconf to produce the configure script
#
# (c) 2009 Prof Dr Andreas Mueller, Hochschule Rapperswil
# $Id$
#
AC_PREREQ(2.57)
AC_REVISION([1.0])
AC_INIT
AC_CONFIG_SRCDIR([tracker.c])
AM_INIT_AUTOMAKE(tracker, 1.0)
AC_CONFIG_HEADERS(config.h)

CFLAGS="${CFLAGS} -std=gnu99 -mmcu=attiny25 -fshort-enums -fno-inline-small-functions -fpack-struct -fno-strict-aliasing -funsigned-char -funsigned-bitfields -ffunction-sections"

AM_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_PROG_CC

AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h stdio.h])

AC_C_CONST

AC_CHECK_FUNCS([memset strdup])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
