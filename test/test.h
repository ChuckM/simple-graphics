/*
 * tests.h
 *
 * Defines some letters as 'colors' for the test
 * programs to use.
 *
 * Declare a screen buffer
 *
 * Declare a pixel drawing function for the buffer.
 *
 * Declare a screen printing function.
 */
#pragma once
#include <gfx.h>

GFX_COLOR letters[] = {
	(GFX_COLOR) { .raw=(uint32_t)'0'},	/* 0 */
	(GFX_COLOR) { .raw=(uint32_t)'1'},	/* 1 */
	(GFX_COLOR) { .raw=(uint32_t)'2'},	/* 2 */
	(GFX_COLOR) { .raw=(uint32_t)'3'},	/* 3 */
	(GFX_COLOR) { .raw=(uint32_t)'4'},	/* 4 */
	(GFX_COLOR) { .raw=(uint32_t)'5'},	/* 5 */
	(GFX_COLOR) { .raw=(uint32_t)'6'},	/* 6 */
	(GFX_COLOR) { .raw=(uint32_t)'7'},	/* 7 */
	(GFX_COLOR) { .raw=(uint32_t)'8'},	/* 8 */
	(GFX_COLOR) { .raw=(uint32_t)'9'},	/* 9 */
	(GFX_COLOR) { .raw=(uint32_t)'A'},	/* 10 */
	(GFX_COLOR) { .raw=(uint32_t)'B'},	/* 11 */
	(GFX_COLOR) { .raw=(uint32_t)'C'},	/* 12 */
	(GFX_COLOR) { .raw=(uint32_t)'D'},	/* 13 */
	(GFX_COLOR) { .raw=(uint32_t)'E'},	/* 14 */
	(GFX_COLOR) { .raw=(uint32_t)'F'},	/* 15 */
	(GFX_COLOR) { .raw=(uint32_t)' '},	/* 16 */
	(GFX_COLOR) { .raw=(uint32_t)'*'},	/* 17 */
	(GFX_COLOR) { .raw=(uint32_t)'@'},	/* 18 */
	(GFX_COLOR) { .raw=(uint32_t)'o'},	/* 19 */
	(GFX_COLOR) { .raw=(uint32_t)'.'},	/* 20 */
	(GFX_COLOR) { .raw=(uint32_t)'+'},	/* 21 */
	(GFX_COLOR) { .raw=(uint32_t)'#'},	/* 22 */
	(GFX_COLOR) { .raw=(uint32_t)'x'},	/* 23 */
	(GFX_COLOR) { .raw=(uint32_t)'-'}	/* 24 */
};

#define C_ONE	letters[1]
#define C_TWO	letters[1]
#define C_THREE	letters[1]
#define C_BLANK	letters[16]
#define C_STAR	letters[17]
#define C_PLUS	letters[21]
#define C_HASH	letters[22]
#define C_EX	letters[23]
#define C_DASH	letters[24]
#define C_AT	letters[18]
#define C_DOT	letters[20]

#ifndef SWIDTH
#define SWIDTH 128
#endif

#ifndef SHEIGHT
#define SHEIGHT 64 
#endif

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
