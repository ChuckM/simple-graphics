/*
 * Simple Graphics - Tiny Font Display
 *
 * Copyright (c) 2014-2017 Charles McManis, all rights reserved.
 *
 * Display all of the characters in the "tiny"
 * font so that you can verify the bit positions
 * are correct and they look correct.
 */

#include <stdint.h>
#include <stdio.h>

/* character box is 4 x 6 so screen is 96 x 8 rows of 8 */
#define SWIDTH 16*4 
#define SHEIGHT 8*6

#include "test.h"

int
main(int argc, char *argv[]) {
	uint8_t a;
	int	line;
	GFX_CTX *g;

	printf("Functional Test: Dump Tiny Font\n");
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_TINY, (void *) screen);
	gfx_fill_screen(g, C_BLANK);
	gfx_set_text_color(g, C_DOT, C_BLANK);
	line = -1;
	for (a = 0; a < 128; a++) {
		if ((a % 16) == 0) {
			line += gfx_get_text_height(g);
			gfx_set_text_cursor(g, 0, line);
		}
		gfx_putc(g, a);
	}
	print_screen();
}
