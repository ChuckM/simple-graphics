/*
 * Simple Graphics - Small Font Display
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 *
 * Display all of the characters in the "small"
 * font so that you can verify the bit positions
 * are correct and they look correct.
 */

#include <stdint.h>
#include <stdio.h>
#include "gfx.h"

#define SWIDTH 128
#define SHEIGHT 32

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
	char buf[260];
	int a;
	char *t;

	printf("Tiny font:\n");
	

	gfx_init(draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_TINY);
	gfx_setFont(GFX_FONT_TINY);
	gfx_fillScreen((uint16_t) ' ');
	gfx_setTextColor((uint16_t) '@', (uint16_t) ' ');
	gfx_setCursor(0, 7);
	gfx_setTextWrap(1);

	for (a = 0; a < 128; a++) {
		gfx_putc((char) a);
		if (((a+1) & 0x1f) == 0) {
			gfx_setCursor(0, 7 + ((a >> 5)+1) * 7);
		}
	}
	print_screen();
}
