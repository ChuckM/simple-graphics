#
# Simple Graphics - Tests
#
# Copyright (c) 2014 - 2015, Charles McManis, all rights reserved.
#
# This code is made available under a Creative Commons 4.0 International
# Public License, see http://creativecommons.org/licenses/by/4.0/legalcode
# for details.
#
# Some simple basic function tests to verify
# the library is doing what is expected. It
# uses "ASCII graphics" and you really should
# have your X term wider than 96 characters to
# view the results. 
#
TESTS= basic rotation large-font small-font

CFLAGS= -g -I ../include 
LDFLAGS= -L ../lib
LDLIBS= -lgfx

all:	$(TESTS)

clean:
	rm -f $(TESTS)
