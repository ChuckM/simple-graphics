#
# Simple Graphics
#
# Copyright (c) 2014 - 2015, Charles McManis, all rights reserved.
#
# This code is made available under a Creative Commons 4.0 International
# Public License, see http://creativecommons.org/licenses/by/4.0/legalcode
# for details.
#
# Generic Makefile for the graphics code
#

CC= gcc
AR= ar

all: lib/libgfx.a

lib/libgfx.a: obj lib/src/gfx.c lib/src/fonts.c include/gfx.h
	$(CC) $(CFLAGS) -g -c lib/src/gfx.c -I ./include -o obj/gfx.o
	$(CC) $(CFLAGS) -g -c lib/src/fonts.c -I ./include -o obj/fonts.o
	$(AR) -rv -o lib/libgfx.a obj/gfx.o obj/fonts.o

obj:
	mkdir ./obj

clean:
	rm -rf obj
	rm -f lib/libgfx.a

