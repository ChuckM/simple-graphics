/*
 * Simple Graphics - Rectangle Test
 *
 * Copyright (c) 2014-2017 Charles McManis, all rights reserved.
 *
 * Debugging filled triangles.
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gfx.h"
#include "colors.h"

#define SWIDTH	100 
#define SHEIGHT	60

uint8_t screen[SWIDTH * SHEIGHT];

void
draw_pixel(void *fb, int x, int y, GFX_COLOR color) {
	uint8_t *s = fb;
	*(s + y*SWIDTH + x) = color.raw & 0xff;
}

/* print out the screen contents */
void print_screen(void) {
	int i, j;
	for (i = 0; i < SHEIGHT; i++) {
		for (j = 0; j < SWIDTH; j++) {
			printf("%c", (char) screen[i*SWIDTH + j]);
		}
		printf("\n");
	}
}


/*
 * Run through the basic operations of the graphics library
 * to verify they work correctly.
 */
int
main(int argc, char *argv[]) {
	int i, a, b;
	int mode = 0;
	GFX_CTX *g;
	printf("Triangle Testing\n");
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *)screen);
	if ((argc == 2)) { 
		mode = atoi(argv[1]);
	}

	gfx_fill_screen(g, C_BLANK);
	gfx_draw_rectangle(g, SWIDTH, SHEIGHT, C_HASH);
	gfx_move(g, 5, 5);
	gfx_draw_rounded_rectangle(g, 40, 20, 7, C_STAR);
	gfx_move(g, 41, 0);
	gfx_draw_rectangle(g, 40, 20, C_HASH);
	gfx_move_to(g, 5, 26);
	gfx_draw_rectangle(g, 40, 20, C_HASH);
	gfx_move(g, 41, 0);
	gfx_fill_rounded_rectangle(g, 40, 20, 7, C_STAR);
	print_screen();
	return 0;

}
