/*
 * Simple Graphics - Digit Tests
 *
 * Copyright (c) 2018 Charles McManis, all rights reserved.
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
#include "test.h"

#define DISP_HEIGHT 100
#define DISP_WIDTH (DISP_HEIGHT / 2.0)
#define SEG_THICK 15
#define SEG_GAP 1
#define H_SEG_GAP (SEG_GAP / 2.0)
#define H_DISP_HEIGHT (DISP_HEIGHT / 2.0)
#define R_SEG_GAP ((SEG_GAP * S2)/ 2.0)

/*
 * There are four unique Segments
 */
float struct _seg {
	int		pts;	/* How many points in the segment */
	float	x, y;	/* Reference point */
	struct {
		float	x, y;	/* relative to reference point */
	} vertices[6];
} segments[7];

/*
 * Segments are 0 - 6 
 *
 * 		  2
 *		----
 *  1  / 6 / 3
 *		---
 *  0 / 5 / 4
 *    ----
 *
 */

#define S2	1.414213562		/* square root of 2 */
struct {
	struct {
		float c[2];
	} pt[6];
} segs[7] =	{
		{ H_SEG_THICK,		0.0 }, /* abs */
		{ -H_SEG_THICK,		-H_SEG_THICK },
		{ -H_SEG_THICK,		- ( H_DISP_HEIGHT - H_SEG_THICK - R_SEG_GAP ) },
		{ 0,				- ( H_DISP_HEIGHT - R_SEG_GAP ) },
		{ H_SEG_THICK,		- ( H_DISP_HEIGHT - H_SEG_THICK - R_SEG_GAP ) },
		{ H_SEG_THICK,		0.0}
	}, {
		{ H_SEG_THICK,		- ( H_DISP_HEIGHT + R_SEG_GAP ) }, /* abs */
		{ -H_SEG_THICK,		- H_SEG_THICK },
		{ -H_SEG_THICK,		- ( H_DISP_HEIGHT - H_SEG_THICK - R_SEG_GAP ) },
		{ 0.0,				- ( H_DISP_HEIGHT - R_SEG_GAP ) },
		{ H_SEG_THICK,		- ( H_DISP_HEIGHT - R_SEG_GAP ) },
		{ H_SEG_THICK, 		- H_SEG_THICK }
	}, {
		{ SEG_THICK + H_SEG_GAP, 								DISP_HEIGHT - 1 }, /* abs */
		{ ( DISP_WIDTH - 1 ) - ( SEG_GAP + SEG_THICK) * 2.0,	0.0 },
		{ ( DISP_WIDTH - 1 ) - ( SEG_GAP + SEG_THICK) * 2.0,	SEG_THICK },
		{ 0.0,													SEG_THICK },
		{ 0, 0},
		{ 0, 0}
	}, {
		{ DISP_WIDTH - 1 - SEG_WIDTH,	DISP_HEIGHT - 1 }, /* abs */
		{ H_SEG_THICK,					0.0 },
		{ SEG_THICK,					-H_SEG_THICK },
		{ SEG_THICK,					-H_DISP_HEIGHT - H_SEG_THICK - R_SEG_GAP },
		{

static void
populate_structs(void)
{
	/* Segment 0 lower left */
	segments[0].vertices[0].x = H_SEG_THICK;
	segments[0].vertices[0].y = 0;					/* A */
	segments[0].vertices[1].x = 0;
	segments[0].vertices[1].y = H_SEG_THICK;	/* B */
	segments[0].vertices[2].x = 0;
	segments[0].vertices[2].y = (DISP_HEIGHT / 2.0) - (SEG_THICK / 2.0) - (SEG_GAP * S2); /* C */
	segments[0].vertices[3].x = SEG_THICK / 2.0;
	segments[0].vertices[3].y = DISP_HEIGHT / 2.0 - (SEG_GAP * S2); /* D */
	segments[0].vertices[4].x = SEG_THICK;
	segments[0].vertices[4].y = DISP_HEIGHT / 2.0 - (SEG_GAP * S2); /* E */
	segments[0].vertices[5].x = SEG_THICK;
	segments[0].vertices[5].y = 0; 									/* F */
	
	

		
}
/*
 * Generate a parameterized 7 segment display
 */
int
main(int argc, char *argv[]) {
	int a, b;
	GFX_CTX *g;
	printf("Testing the graphics code\n");
	
	g = gfx_init(NULL, draw_pixel, SWIDTH, SHEIGHT, GFX_FONT_LARGE, (void *)screen);
	gfx_fill_screen(g, C_BLANK);

	/* 
	 * draw the same line with rotation and rotation
	 * origin set.
	 * This tests:
	 *   -- paint_pixel (it has to translate)
	 *	 -- paint_line (short cut code)
	 *	 -- gfx_move_to
	 *	 -- gfx_rotation
	 *	 -- gfx_set_rotation_origin.
	 */
	gfx_set_rotation_origin(g, 10, 10);
	gfx_move_to(g, 10, 10);
	gfx_draw_line(g, 25, 0, letters[1]);
	gfx_rotate(g, 45.0);
	gfx_move_to(g, 10, 10);
	gfx_draw_line(g, 25, 0, letters[2]);
	gfx_rotate(g, 90.0);
	gfx_move_to(g, 10, 10);
	gfx_draw_line(g, 25, 0, letters[3]);
	gfx_rotate(g, 0);

	/*
	 * draw a triangle and a filled triangle.
	 * This tests:
	 *	  -- gfx_move (relative move)
	 *	  -- paint_line (bresenham code)
	 *	  -- paint_triangle
	 *	  -- gfx_draw_triangle
	 *	  -- gfx_fill_triangle
	 */
	gfx_move_to(g, 10, 25);
	gfx_draw_triangle(g, 10, 10, 10, 0, C_DOT);
	gfx_draw_point(g, letters[10]);
	gfx_move(g, 10, 10);
	gfx_draw_point(g, letters[11]);
	gfx_move(g, -10, -10);
	gfx_move(g, 10, 0);
	gfx_draw_point(g, letters[12]);
	gfx_move(g, -10, 0);
	
	gfx_move(g, 25, 0);
	gfx_fill_triangle(g, 10, 10, 10, 0, C_STAR);
	gfx_draw_point(g, letters[10]);
	gfx_move(g, 10, 10);
	gfx_draw_point(g, letters[11]);
	gfx_move(g, -10, -10);
	gfx_move(g, 10, 0);
	gfx_draw_point(g, letters[12]);
	gfx_move(g, -10, 0);

	gfx_move_to(g, 10, 23);
	gfx_draw_triangle(g, 10, -10, 10, 0, C_DOT);
	gfx_draw_point(g, letters[10]);
	gfx_move(g, 10, -10);
	gfx_draw_point(g, letters[11]);
	gfx_move(g, -10, 10);
	gfx_move(g, 10, 0);
	gfx_draw_point(g, letters[12]);
	gfx_move(g, -10, 0);
	
	gfx_move(g, 25, 0);
	gfx_fill_triangle(g, 10, -10, 10, 0, C_STAR);
	gfx_draw_point(g, letters[10]);
	gfx_move(g, 10, -10);
	gfx_draw_point(g, letters[11]);
	gfx_move(g, -10, 10);
	gfx_move(g, 10, 0);
	gfx_draw_point(g, letters[12]);
	gfx_move(g, -10, 0);

	/*
	 * draw a circle and a filled circle.
	 * This tests:
	 *	  -- paint_quadrant
	 *	  -- gfx_draw_circle
	 *	  -- gfx_fill_circle
	 */
	gfx_move_to(g, 70, 16);
	gfx_draw_circle(g, 15, C_STAR);
	gfx_move(g, 0, 30);
	gfx_fill_circle(g, 15, C_DOT);
	/*
	 * draw a rectangle and a filled rectangle
	 * This tests:
	 *	  -- gfx_draw_rectangle
	 *	  -- gfx_fill_rectangle
	 */
	gfx_move_to(g, 0, 0);
	gfx_draw_rectangle(g, SWIDTH, SHEIGHT, C_HASH);
	gfx_move_to(g, 25, 50);
	gfx_draw_rectangle(g, 20, 10, letters[0]);
	gfx_fill_rectangle(g, 20, -10, letters[1]);
	gfx_fill_rectangle(g, -20, 10, letters[2]);
	gfx_draw_rectangle(g, -20, -10, letters[3]);
	print_screen();
	return 0;

	gfx_move_to(g, 17, 3);
	gfx_fill_rectangle(g, 66, 14, C_HASH);
#if 0
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
#endif
	/* draw filled and outline versions of shapes */
	gfx_move_to(g, 16,38);
	gfx_draw_circle(g,5, C_STAR);
	gfx_move_to(g, 25,33);
	gfx_fill_rectangle(g, 10, 10, C_AT);
	gfx_move_to(g, 38 , 43);
	gfx_draw_triangle(g, 48, 43, 42, 33, C_PLUS);

	gfx_move_to(g, 11, 55);
	gfx_fill_triangle(g, 21, 55, 16, 45, C_PLUS);
	gfx_move_to(g, 25, 45);
	gfx_draw_rectangle(g, 10, 10, C_AT);
	gfx_move_to(g, 43, 50);
	gfx_fill_circle(g, 5, C_STAR);

	/* check for proper edge filling on adjacent triangles */
	gfx_move_to(g, 52, 33);
	gfx_fill_triangle(g, 20, 0, 20, 10, C_DOT); 
	gfx_fill_triangle(g, 0, 20, 20, 10, C_AT);

#if 0
	/* rotate the other way */
	gfx_setTextRotation(GFX_ROT_90);
	gfx_setCursor(80, 33);
	gfx_puts("Test");
#endif
	print_screen();
}
