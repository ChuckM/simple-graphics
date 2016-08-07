/*
 * Simple Graphics - basic tests
 *
 * Copyright (c) 2014-2016 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 * 
 * Triangle filling test. This exercises the triangles filling
 * through all combinations of CW and CCW winding
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "gfx.h"

#define SWIDTH 100
#define SHEIGHT 60 

uint8_t screen[SWIDTH * SHEIGHT];

void
draw_pixel(int x, int y, uint16_t color) {
	screen[y*SWIDTH + x] = color & 0xff;
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
 * Test code
 */
int
main(int argc, char *argv[]) {
	int a, b;
	int x, y;
	printf("Triangle Fill test.\n");
	
	gfx_init(draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE);
	if ((argc == 2) && (strncmp(argv[1], "-m", 2) == 0)) {
		/* 
	 	 * set mirrored, to graphics are correct when seen
		 * reflected in a mirror. Useful when building a
		 * heads up type display where the actual display
 		 * is reflected.
		 */
		gfx_setMirrored(1);
	}

	gfx_fillScreen((uint16_t) ' ');
	/* triangles are 25 pix high, 25 pix wide, spaced every 30 pix */
	/* test rectangle full screen size */

/*
 * Triangle :
 *             B
 *             | \
 *             |  \
 *             |   C
 *             |  /
 *             | /
 *             A
 *
 * Test draw it CW rotation of points: A-B-C, B-C-A, C-A-B
 */
	y = 25;
	x = 10;
	gfx_fillTriangle(
					 0 + x,       y,		/* A */
					 0 + x ,      y - 25,	/* B */
					 0 + x + 25,  y - 13, 	/* C */
					 (uint16_t) '*');
	gfx_fillTriangle(
					 30 + x ,      y - 25,	/* B */
					 30 + x + 25,  y - 13, 	/* C */
					 30 + x,       y,		/* A */
					 (uint16_t) '*');
	gfx_fillTriangle(
					 60 + x + 25,  y - 13, 	/* C */
					 60 + x,       y,		/* A */
					 60 + x ,      y - 25,	/* B */
					 (uint16_t) '*');

/*
 * Triangle :
 *             B
 *             | \
 *             |  \
 *             |   C
 *             |  /
 *             | /
 *             A
 *
 * Test draw it A-C-B, C-B-A, B-A-C
 */
	y = 55;
	gfx_fillTriangle(
					 0 + x,       y,		/* A */
					 0 + x + 25,  y - 13, 	/* C */
					 0 + x ,      y - 25,	/* B */
					 (uint16_t) '*');
	gfx_fillTriangle(
					 30 + x + 25, y - 13,	/* C */
					 30 + x      ,y - 25,	/* B */
					 30 + x      ,y,		/* A */
					 (uint16_t) '*');
	gfx_fillTriangle(
					 60 + x ,      y - 25,	/* B */
					 60 + x ,      y,		/* A */
					 60 + x + 25,  y - 13,	/* C */
					 (uint16_t) '*');
	print_screen();
}
