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

#
# To just build generically uncomment out these
#
#CC= gcc
#AR= ar

#
# To build for a Cortex M4 target uncomment these
#
CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar
CFLAGS += -mthumb -mcpu=cortex-m4 
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

CFLAGS += -Os -g -I./include 

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

