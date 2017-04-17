/*
 * Simple Graphics - Triangle Tests
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

#define SWIDTH	60 
#define SHEIGHT	30

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

static void
test_triangle(GFX_CTX *g, int ax, int ay, int bx, int by, int cx, int cy)
{
	gfx_move_to(g, ax, ay);
	gfx_fill_triangle(g, bx, by, cx, cy, C_STAR);
	gfx_draw_point(g, letters[10]);
	gfx_move_to(g, ax+bx, ay+by);
	gfx_draw_point(g, letters[11]);
	gfx_move_to(g, ax+cx, ay+cy);
	gfx_draw_point(g, letters[12]);
	gfx_move_to(g, ax, ay);
	gfx_move(g, 30, 0);
	gfx_draw_triangle(g, bx, by, cx, cy, C_HASH);
	gfx_draw_point(g, letters[10]);
	gfx_move(g, bx, by);
	gfx_draw_point(g, letters[11]);
	gfx_move(g, -bx, -by);
	gfx_move(g, cx, cy);
	gfx_draw_point(g, letters[12]);
	gfx_move_to(g, ax, ay);
	
}

/*
 * Run through the basic operations of the graphics library
 * to verify they work correctly.
 */
int
main(int argc, char *argv[]) {
	int a, b;
	int mode = 0;
	GFX_CTX *g;
	printf("Triangle Testing\n");
	
	g = gfx_init(draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *)screen);
	if ((argc == 2)) { 
		mode = atoi(argv[1]);
	}

	memset(screen, ' ', sizeof(screen));
//	gfx_fill_screen(g, C_BLANK);
	gfx_draw_rectangle(g, SWIDTH, SHEIGHT, C_HASH);

	switch (mode) {
		default:
			test_triangle(g, 5, 5, 15, 15, 15, 0);
			break;
		case 1:
			test_triangle(g, 5, 15, 10, 10, 20, 0);
			break;
		case 2:
			test_triangle(g, 15, 5, -10, 10, 10, 10);
			break;
		case 3:
			test_triangle(g, 15, 5, 10, 10, -10, 10);
			break;
		case 4:
			test_triangle(g, 25, 15, -10, -10, -20, 0);
			break;
	}

	print_screen();
	return 0;

}
