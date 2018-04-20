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
