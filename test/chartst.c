/*
 * Simple Graphics - character tests
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
#include <stdlib.h>
#include <string.h>
#include "test.h"

/*
 * Run through the basic operations of the graphics library
 * to verify they work correctly.
 */
int
main(int argc, char *argv[]) {
	int a, b;
	int mode = 0;
	GFX_CTX *g;
	printf("Testing the %s graphics functions\n", argv[0]);
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *)screen);
	if (argc == 2) {
		mode = atoi(argv[1]);
	}

	memset(screen, ' ', sizeof(screen));
	gfx_draw_rectangle(g, SWIDTH, SHEIGHT, C_HASH);
	gfx_set_text_color(g, C_AT, C_DOT);
	gfx_set_text_cursor(g, 2, 15);
	gfx_move_to(g, 2, 15);
	switch (mode) {
		default:
			gfx_set_text_rotation(g, 0);
			gfx_draw_glyph(g, 'A', C_AT, C_DOT);
			gfx_draw_line(g, 0, 10, C_STAR);
			break;
		case 2:
			gfx_set_text_rotation(g, -90);
		case 1:
			gfx_putc(g, 'H');
			gfx_putc(g, 'e');
			gfx_putc(g, 'l');
			gfx_putc(g, 'l');
			gfx_putc(g, 'o');
			gfx_putc(g, '!');
			break;
		case 5:
			gfx_set_text_size(g, 1);
			gfx_set_text_cursor(g, SWIDTH/2, SHEIGHT/2);
			gfx_set_text_rotation(g, -45);
			gfx_draw_glyph(g, 'B', C_AT, C_DOT);
			
		case 4:
			gfx_set_text_rotation(g, -45);
		case 3:
			gfx_set_text_size(g, 4);
			gfx_set_text_cursor(g, 2, 30);
			gfx_draw_glyph(g, 'B', C_AT, C_DOT);
			break;
	}
	print_screen();
	return 0;
}
