/*
 * Simple Graphics - Screen Rotation
 *
 * Copyright (c) 2014-2015 Charles McManis, all rights reserved.
 *
 * This source code is licensed under a Creative Commons 4.0 
 * International Public license. 
 *
 * See: http://creativecommons.org/licenses/by/4.0/legalcode for
 * details.
 *
 * The library tests both rotation of the screen and
 * rotation of the text.
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gfx.h"

#define SWIDTH 100
#define SHEIGHT 60 

uint8_t screen[SWIDTH * SHEIGHT];

void
draw_pixel(int x, int y, uint16_t color) {
	if (x >= SWIDTH) {
//		printf("Pixel map failure? got pixel %d for x\n", x);
		return;
	}
	if (y >= SHEIGHT) {
//		printf("Pixel map failure? got pixel %d for y\n", y);
		return;
	}
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
	int a, b;
	enum gfx_rotate r = GFX_ROT_0;

	printf("Testing the graphics code\n");
	if (argc == 3) {
		int angle;
		if (strncmp(argv[1], "-r", 2)) {
			fprintf(stderr, "usage: rotation [-s 0|90|180|270]\n");
			exit(1);
		}
		angle = atoi(argv[2]);
		switch (angle) {
			case 90:
				r = GFX_ROT_90;
				break;
			case 180:
				r = GFX_ROT_180;
				break;
			case 270:
				r = GFX_ROT_270;
				break;
			default:
				break;
		}
	}

	gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE);
	gfx_setRotation(r);
	printf("New screen height, width is %d, %d\n", gfx_height(), gfx_width());
	gfx_fillScreen((uint16_t) ' ');
	gfx_setTextColor((uint16_t) '@', (uint16_t) ' ');
	gfx_setCursor(5, 15);
	gfx_setTextRotation(GFX_ROT_0);
	gfx_puts("Rotation ");
	gfx_setTextRotation(GFX_ROT_90);
	gfx_puts("is ");
	gfx_setTextRotation(GFX_ROT_180);
	gfx_puts("useful ");
	gfx_setTextRotation(GFX_ROT_270);
	gfx_puts("eh?");
	gfx_setTextRotation(GFX_ROT_0);
	gfx_drawRoundRect(28,20, 34, 11, 3, (uint16_t) '-');
	gfx_setTextColor((uint16_t) '*', (uint16_t) '*');
	gfx_setCursor(34, 30);
	gfx_puts("Up\034");

	print_screen();
}
