#
# Makefile.am -- build the software for the tracker
#
# (c) 2015 Prof Dr Andreas Mueller, Hochschule Rapperswil
#
# LUFA configuration stuff

noinst_PROGRAMS = tracker

AVRDUDE = avrdude -v -p t25 -b 115200 -c avrispmkII -P usb

flash:	tracker.hex
	$(AVRDUDE) -U flash:w:tracker.hex
eeprom:	tracker.eeprom
	$(AVRDUDE) -U eeprom:w:tracker.eeprom

tracker.hex: tracker
	avr-objcopy --remove-section=.eeprom --target=ihex tracker \
		tracker.hex
tracker.eeprom: tracker
	avr-objcopy --only-section=.eeprom --target=ihex tracker \
		tracker.eeprom

lib_LTLIBRARIES = libtracker.la

noinst_HEADERS = button.h timer.h direction.h

libtracker_la_SOURCES = button.c timer.c direction.c

tracker_SOURCES = tracker.c
tracker_DEPENDENCIES = libtracker.la
tracker_LDFLAGS = -ltracker

# setting the fuse values
# low fuse byte
# -------------
# CKDIV8        = 0             Clock divided by 8 (yes)
# CKOUT         =  1            Clock out enabled (no)
# SUT1..0       =   10          Startup time setting (fast rising power)
# CKSEL3..0     =     0010      Clock source setting (calibrated RC Oscillator)
#                 ________
#                 01100010      = 0x62
#
#
# high fuse byte
# --------------
# DWEN          = 1             DebugWIRE enabled (no)
# RSTDSBL       =  1            external reset disabled (no)
# SPIEN         =   0           Serial program and data download enabled (yes)
# WDTON         =    1          Watchdog timer always on (no)
# EESAVE        =     0         EEPROM preserved on chip erase (yes)
# BODLEVEL2..0  =      111      BOD disabled
#                 ________
#                 11010111      = 0xd7
#
#
# extended fuse byte
# ------------------
# LB2..LB1      =       11      No memory lock features enabled
#                 ________
#                 00000011      = 0x03
#
# original fuse settings reported by avrdude
# lfuse reads as 5E
# hfuse reads as D9
# efuse reads as F4

# The fuse command below does not set the extended fuses, as there is no
# need to change the factory settings
fuses:
	$(AVRDUDE) -U lfuse:w:0x62:m -U hfuse:w:0xd7:m

read:   readback.hex

readback.hex:
	$(AVRDUDE) -U flash:r:readback.hex

read:
	$(AVRDUDE) -U eeprom:r:eeprom.raw:r -u

size:   tracker
	avr-size *.o tracker

