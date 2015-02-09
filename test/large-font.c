/*
 * Simple Graphics - Large Font Display
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 *
 * Display all of the characters in the "large"
 * font so that you can verify the bit positions
 * are correct and they look correct.
 */

#include <stdint.h>
#include <stdio.h>
#include "gfx.h"

#define SWIDTH 96
#define SHEIGHT 11*12

uint8_t screen[SWIDTH * SHEIGHT];

void
draw_pixel(int x, int y, uint16_t color) {
	screen[y*SWIDTH + x] = color & 0xff;
}

void print_screen(void) {
	int i, j;
	for (i = 0; i < SHEIGHT; i++) {
		for (j = 0; j < SWIDTH; j++) {
			printf("%c", (char) screen[i*SWIDTH + j]);
		}
		printf("\n");
	}
}

int
main(int argc, char *argv[]) {
	uint8_t a;
	char buf[260];
	char *t;

	printf("Testing the graphics code\n");
	

	gfx_init(draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE);
	gfx_fillScreen((uint16_t) ' ');
	gfx_setTextColor((uint16_t) '@', (uint16_t) ' ');
	gfx_setCursor(0, 9);
	gfx_setTextWrap(1);
	for (a = 1, t = &buf[0]; a < 128; a++, t++) {
		*t = (char) a;
	}
	*t = 0;
	gfx_puts(buf);
	gfx_write(0);
	print_screen();
}
