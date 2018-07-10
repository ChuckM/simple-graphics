/*
 * Simple Graphics - Rotation test
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

#define SWIDTH	60 
#define SHEIGHT	60

#include "test.h"


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
	gfx_move(g, 0, 30);
	for (i = 0; i < 360; i += 25) {
		gfx_draw_rectangle(g, 25, -12, C_DOT);
		gfx_rotate(g, i);
	}
	print_screen();
	return 0;

}
