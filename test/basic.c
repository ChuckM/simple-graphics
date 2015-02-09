/*
 * Simple Graphics - basic tests
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 * 
 * This is the basic function test it uses most
 * of the functions in the library to create a 
 * test graphic.
 *
 */

#include <stdint.h>
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
 * Run through the basic operations of the graphics library
 * to verify they work correctly.
 */
int
main(int argc, char *argv[]) {
	int a, b;
	printf("Testing the graphics code\n");
	
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
	/* test rectangle full screen size */
	gfx_drawRoundRect(0, 0, 100, 60, 5, (uint16_t) '*');
	gfx_fillRect(17, 3, 66, 14, (uint16_t) '#');
	gfx_setCursor(19, 13);
	/* this text doesn't write the 'background' color */
	gfx_setTextColor((uint16_t) ' ', (uint16_t) ' ');
	gfx_puts("Graphics");

	/* change font size on the fly */
	gfx_setFont(GFX_FONT_SMALL);
	gfx_setTextColor((uint16_t) '@', (uint16_t) ' ');
	gfx_setCursor(8, 32);
	/* multiplicative scaling for 'bigger' text */
	gfx_setTextSize(2);
	gfx_puts("Testing");
	gfx_setTextSize(1);
	/* text rotation (four possible angles) */
	gfx_setTextRotation(GFX_ROT_270);
	gfx_setCursor(10, 55);
	gfx_puts("Geom");
	/* draw filled and outline versions of shapes */
	gfx_drawCircle(16, 38, 5, (uint16_t) '*');
	gfx_fillRect(25, 33, 10, 10, (uint16_t) 'o');
	gfx_drawTriangle(38, 43, 48, 43, 42, 33, (uint16_t) '+');

	gfx_fillTriangle(11, 55, 21, 55, 16, 45, (uint16_t) '+');
	gfx_drawRect(25, 45, 10, 10, (uint16_t) 'o');
	gfx_fillCircle(43, 50, 5, (uint16_t) '*');

	/* check for proper edge filling on adjacent triangles */
	gfx_fillTriangle(52, 33, 77, 33, 52, 55, (uint16_t) '@');
	gfx_fillTriangle(52, 55, 77, 33, 77, 55, (uint16_t) 'O');

	/* rotate the other way */
	gfx_setTextRotation(GFX_ROT_90);
	gfx_setCursor(80, 33);
	gfx_puts("Test");
	print_screen();
}
