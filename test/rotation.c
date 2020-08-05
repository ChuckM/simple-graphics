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
#include "test.h"

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
	gfx_fillScreen(C_SPACE);
	gfx_setTextColor(C_AT, C_SPACE);
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
	gfx_drawRoundRect(28,20, 34, 11, 3, C_DASH);
	gfx_setTextColor(C_STAR, C_STAR);
	gfx_setCursor(34, 30);
	gfx_puts("Up\034");

	print_screen();
}
