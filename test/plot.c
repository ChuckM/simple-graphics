/*
 * Simple Graphics - Plotting Test
 *
 * Copyright (c) 2014-2017 Charles McManis, all rights reserved.
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
#include <string.h>
#include <math.h>
#include "test.h"

/*
 * Run through the basic operations of the graphics library
 * to verify they work correctly.
 */
int
main(int argc, char *argv[]) {
	int a, b;
	float i, x0, y0;
	GFX_CTX *g;
	GFX_VIEW *vp;
	printf("Testing the graphics viewport code\n");
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *)screen);
	gfx_fill_screen(g, C_BLANK);
	vp = gfx_viewport(NULL, g, 0, 0, SWIDTH, SHEIGHT, 0, -1.0, 2 * M_PI, 1.0);
	x0 = 0;
	y0 = sin(0);
	for (i = 2 * M_PI / 100.0; i < 2 * M_PI; i += 2 * M_PI / 100.0) {
		vp_plot(vp, x0, y0, i, sin(i), C_STAR);
		x0 = i;
		y0 = sin(i);
	}
	print_screen();
}
