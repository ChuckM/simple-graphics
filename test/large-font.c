/*
 * Simple Graphics - Large Font Display
 *
 * Copyright (c) 2014-2017 Charles McManis, all rights reserved.
 *
 * Display all of the characters in the "large"
 * font so that you can verify the bit positions
 * are correct and they look correct.
 */

#include <stdint.h>
#include <stdio.h>
#include "gfx.h"
#include "colors.h"

/* character box is 8 x 12 so screen is 128 x 96 (8 rows of 16) */
#define SWIDTH 8*16
#define SHEIGHT 12*8

uint8_t screen[SWIDTH * SHEIGHT];

void
draw_pixel(void *fb, int x, int y, GFX_COLOR color) {
	screen[y*SWIDTH + x] = color.raw & 0xff;
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
	int	line;
	GFX_CTX *g;

	printf("Functional Test: Dump Large Font\n");
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *) screen);
	gfx_fill_screen(g, C_BLANK);
	gfx_set_text_color(g, C_AT, C_BLANK);
	gfx_set_text_cursor(g, 0, 9);
	line = -3;
	for (a = 0; a < 128; a++) {
		if ((a % 16) == 0) {
			line += gfx_get_text_height(g);
			gfx_set_text_cursor(g, 0, line);
		}
		gfx_putc(g, a);
	}
	print_screen();
}
