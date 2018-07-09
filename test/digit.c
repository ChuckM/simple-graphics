/*
 * Digit drawing test
 *
 * Copyright (C) 2018 Chuck McManis, All rights reserved.
 *
 * There is something weird going on here and this example
 * triggers it. Basically it renders the digit incorrectly
 * and then corrupts memory.
 */

#include <stdio.h>
#include <stdlib.h>
#include <gfx.h>
#include "test.h"

void draw_digit(GFX_CTX *g, int x, int y, int d, GFX_COLOR color, GFX_COLOR outline);
void draw_colon(GFX_CTX *g, int x, int y, GFX_COLOR color, GFX_COLOR outline);
void draw_dp(GFX_CTX *g, int x, int y, GFX_COLOR color, GFX_COLOR outline);
void generate_digits(void);

/*
 *   This defines a parameterized 7 segment display graphic
 *   overall graphic is DISP_WIDTH pixels wide by DISP_HEIGHT
 *   pixels tall.
 *
 *   Segments are SEG_THICK pixels "thick".
 *
 *   Gaps between segments are SEG_GAP pixel(s)
 */

#define DISP_HEIGHT 100
#define DISP_WIDTH (DISP_HEIGHT / 2)
#define SEG_THICK 15
#define SEG_GAP 1
#define SKEW_MAX (DISP_WIDTH / 3.0)
#define SKEW_FACTOR ((SKEW_MAX) / DISP_HEIGHT)

/* Vertical segment (6 co-ordinate pairs) */
static const int v_seg[] = {
/*A*/	0, 0,
/*B*/	SEG_THICK / 2, SEG_THICK / 2,
/*C*/	SEG_THICK / 2, DISP_HEIGHT / 2 - SEG_GAP,
/*D*/	0, DISP_HEIGHT / 2 - SEG_GAP,
/*E*/	-SEG_THICK / 2, ((DISP_HEIGHT / 2) - SEG_GAP) - (SEG_THICK / 2),
/*F*/	-SEG_THICK / 2, SEG_THICK / 2
};

/* Middle segment (6 co-ordinate pairs) */
static const int m_seg[] = {
/*A*/	0, 0,
/*B*/	SEG_THICK / 2, -SEG_THICK / 2,
/*C*/	SEG_THICK / 2 + (DISP_WIDTH - (2 * (SEG_THICK + SEG_GAP))), -SEG_THICK / 2,
/*D*/	(DISP_WIDTH - SEG_THICK - (2 * SEG_GAP)), 0,
/*E*/	SEG_THICK / 2 + (DISP_WIDTH - (2 * (SEG_THICK + SEG_GAP))), SEG_THICK / 2,
/*F*/	SEG_THICK / 2, SEG_THICK / 2
};

/* Horizontal segment (4 co-ordinate pairs) */
static const int h_seg[] = {
/*A*/	0, 0,
/*B*/	0, -SEG_THICK,
/*C*/	DISP_WIDTH - (2 * (SEG_THICK + SEG_GAP)), -SEG_THICK,
/*D*/	DISP_WIDTH - (2 * (SEG_THICK + SEG_GAP)), 0
};

/*
 * This array maps which segments are on for each digit
 * by having a '1' value if the segment is on, and a '0'
 * if the segment is off. The segments are identified
 * in the diagram below:
 *
 *        6
 *       ---
 *   5 / 4 / 3
 *     ---
 * 2 / 1 / 0
 *   ---
 *
 * Segments by bit position.
 */
const uint8_t seg_map[10] = {
	0b01101111,		/* 0 */
	0b00001001,		/* 1 */
	0b01011110,		/* 2 */
	0b01011011,		/* 3 */
	0b00111001,		/* 4 */
	0b01110011,		/* 5 */
	0b01110111,		/* 6 */
	0b01001001,		/* 7 */
	0b01111111,		/* 8 */
	0b01111001		/* 9 */
};

/*
 * This structure gives rendering parameters for each segment.
 * Which set of parameters to use (there are three unique sets)
 * and how to reflect them to meet their orientation requirements.
 * Each segment descriptor has a reference point (co-ordinate "A")
 */
static const struct {
	int	n_coords;	/* number of co-ordinates */
	int	xf;			/* X mirror factor */
	int	yf;			/* Y mirror factor */
	int	xo;			/* X offset */
	int	yo;			/* Y offset */
	const int	*segs;	/* pointer to segment co-ordinates */
} segment_data[7] = {
/*0*/	{ 6, -1, 1, DISP_WIDTH - SEG_THICK / 2, DISP_HEIGHT / 2 + SEG_GAP, &v_seg[0]},
/*1*/	{ 4, 1, 1, SEG_THICK + SEG_GAP, DISP_HEIGHT, &h_seg[0] },
/*2*/	{ 6, 1, 1, SEG_THICK / 2, DISP_HEIGHT / 2 + SEG_GAP, &v_seg[0]},
/*3*/	{ 6, -1, -1, DISP_WIDTH - SEG_THICK / 2, DISP_HEIGHT / 2 - SEG_GAP, &v_seg[0]},
/*4*/	{ 6, 1, 1, SEG_THICK / 2 + SEG_GAP, DISP_HEIGHT / 2, &m_seg[0]},
/*5*/	{ 6, 1, -1, SEG_THICK/2, DISP_HEIGHT/2 - SEG_GAP, &v_seg[0] },
/*6*/	{ 4, 1, 1, SEG_THICK + SEG_GAP, SEG_THICK, &h_seg[0] },
};


/*
 * Compute a "skew factor".
 * Compute a 'delta x' factor to add to the X coordinate
 * when transitioning delta Y pixels. 
 *
 * Slant goes from 0 at dy == h to SKEW_MAX when dy == 0
 */
static int
skew_factor(int dy, int h)
{
	int i;
	/* dy is distance from the 'top' of the display box */
	i =  ((SKEW_MAX * (h - dy)) / h);
	return i;
}

/* gap question, is disp_height everything, including gaps? (same with display width?) */
/* How about display pad, for the "socket" around the display? */
/* How about the decimal point with respect to the digits? */
// #define OUTLINE_DIGIT_BOX (broken)

#define OUTLINE_DIGIT_BOX
/*
 * Draw a graphic that looks like a 7 segment display
 * digit. 
 *
 * The reference point for the digit is the lower left corner
 * of the "display" so digits go up and to the right from that point.
 * Because Y is inverted in this display (larger Y means lower point)
 * Height is "negative" with respect to the starting point. (XXX look
 * this, can we make the graphics "sane" with respect to X and Y and
 * map it in the simple-graphics library appropriately?
 */
void
draw_digit(GFX_CTX *g, int x, int y, int d, GFX_COLOR color, GFX_COLOR outline) {
	int i;
	uint8_t seg_mask;
	int	sx, sy, ndx, xf, yf;

	seg_mask = seg_map[d % 10];

#ifdef OUTLINE_DIGIT_BOX
	gfx_move(g, x - 1, y + 1);
	gfx_draw_line(g, (int) SKEW_FACTOR * (DISP_HEIGHT + 2), -(DISP_HEIGHT+2), C_HASH);
	gfx_draw_line(g, DISP_WIDTH + 2, 0, C_HASH);
	gfx_draw_line(g, (int) - SKEW_FACTOR * (DISP_HEIGHT + 2), DISP_HEIGHT + 2, C_HASH);
	gfx_draw_line(g, - (DISP_WIDTH + 2), 0, C_HASH);
#endif

	for (i = 0; i < 7; i++) {
		/* test to see if segment is 'lit' for this number */
		if ((seg_mask & (1 << i)) != 0) {
			/* draw segment */
			sx = x + segment_data[i].xo;
			sy = y + segment_data[i].yo;
			xf = segment_data[i].xf;
			yf = segment_data[i].yf;
			/* tesselate it */
			for (ndx = 2; ndx < (segment_data[i].n_coords - 1) * 2; ndx += 2) {
				/* move to segment reference point given X, Y, and Skew */
				gfx_move(g, sx, sy);
					
				gfx_fill_triangle_abs(g,
					skew_factor((sy + *(segment_data[i].segs + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs) * xf,
					sy + *(segment_data[i].segs + 1) * yf,			/* #1 */

					skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
						sx	+ *(segment_data[i].segs + ndx) * xf,
					sy + *(segment_data[i].segs + ndx + 1) * yf,	/* #2 */

					skew_factor((sy + *(segment_data[i].segs + ndx + 3) * yf - y), DISP_HEIGHT) +
					     sx + *(segment_data[i].segs + ndx + 2) * xf,
					sy + *(segment_data[i].segs + ndx + 3) * yf,	/* #3 */
							 color);
			}
			/* outline it */
			for (ndx = 0; ndx < (segment_data[i].n_coords - 1) * 2; ndx += 2) {
				gfx_draw_line_at(g,
					skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
						sx + *(segment_data[i].segs + ndx) * xf,
					sy + *(segment_data[i].segs + ndx + 1) * yf,
					skew_factor((sy + *(segment_data[i].segs + ndx + 3) * yf - y), DISP_HEIGHT) +
								sx	+ *(segment_data[i].segs + ndx + 2) * xf,
					sy + *(segment_data[i].segs + ndx + 3) * yf,
								outline);
			}
			gfx_draw_line_at(g,
				skew_factor((sy + *(segment_data[i].segs + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs + 0) * xf,
				sy + *(segment_data[i].segs + 1) * yf,
				skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs + ndx) * xf,
				sy + *(segment_data[i].segs + ndx + 1) * yf,
							outline);
		} else {
			/* draw 'off' segment? */
#ifdef DRAW_DARK_SEGMENT
			/* draw segment */
			sx = x + segment_data[i].xo;
			sy = y + segment_data[i].yo;
			xf = segment_data[i].xf;
			yf = segment_data[i].yf;
			/* tesselate it */
			for (ndx = 2; ndx < (segment_data[i].n_coords - 1) * 2; ndx += 2) {
				gfx_fill_triangle_abs(g,
					skew_factor((sy + *(segment_data[i].segs + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs) * xf,
					sy + *(segment_data[i].segs + 1) * yf,			/* #1 */
					skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
						sx	+ *(segment_data[i].segs + ndx) * xf,
					sy + *(segment_data[i].segs + ndx + 1) * yf,	/* #2 */
					skew_factor((sy + *(segment_data[i].segs + ndx + 3) * yf - y), DISP_HEIGHT) +
					     sx + *(segment_data[i].segs + ndx + 2) * xf,
					sy + *(segment_data[i].segs + ndx + 3) * yf,	/* #3 */
							(uint16_t) (6 << 11 | 6 << 5 | 6)
							 );
			}
			/* outline it */
			for (ndx = 0; ndx < (segment_data[i].n_coords - 1) * 2; ndx += 2) {
				gfx_draw_line_abs(g,
					skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
						sx + *(segment_data[i].segs + ndx) * xf,
					sy + *(segment_data[i].segs + ndx + 1) * yf,
					skew_factor((sy + *(segment_data[i].segs + ndx + 3) * yf - y), DISP_HEIGHT) +
								sx	+ *(segment_data[i].segs + ndx + 2) * xf,
					sy + *(segment_data[i].segs + ndx + 3) * yf,
							outline);
			}
			gfx_draw_line_abs(g,
				skew_factor((sy + *(segment_data[i].segs + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs + 0) * xf,
				sy + *(segment_data[i].segs + 1) * yf,
				skew_factor((sy + *(segment_data[i].segs + ndx + 1) * yf - y), DISP_HEIGHT) +
							sx + *(segment_data[i].segs + ndx) * xf,
				sy + *(segment_data[i].segs + ndx + 1) * yf,
							outline);
#endif
		}
	}
}

/* Colon spread as a percentage of display height */
#define COLON_SPREAD (DISP_HEIGHT * 15 / 100)

/*
 * For 'clock' type displays they have a colon between
 * the Hours, minutes, and seconds. This is our LED
 * colon (:) character.
 *
 * Started putting the 'dots' at 25% and 75% of display
 * height, and switched to equidistant from the display
 * vertical mid-point.
 */
void
draw_colon(GFX_CTX *g, int x, int y, GFX_COLOR color, GFX_COLOR outline)
{
	gfx_fill_circle_at(g,
		skew_factor(DISP_HEIGHT / 2 - COLON_SPREAD, DISP_HEIGHT) + x + SEG_THICK/2,
		y + (DISP_HEIGHT / 2 - COLON_SPREAD) - SEG_THICK/2, SEG_THICK/2, color);
	gfx_draw_circle_at(g,
		skew_factor(DISP_HEIGHT / 2 - COLON_SPREAD, DISP_HEIGHT) + x + SEG_THICK/2,
		y + (DISP_HEIGHT / 2 - COLON_SPREAD) - SEG_THICK/2, SEG_THICK/2, outline);
	gfx_fill_circle_at(g,
		skew_factor(DISP_HEIGHT / 2 + COLON_SPREAD, DISP_HEIGHT) + x + SEG_THICK/2,
		y + (DISP_HEIGHT / 2 + COLON_SPREAD) + SEG_THICK/2, SEG_THICK/2, color);
	gfx_draw_circle_at(g,
		skew_factor(DISP_HEIGHT / 2 + COLON_SPREAD, DISP_HEIGHT) + x + SEG_THICK/2,
		y + (DISP_HEIGHT / 2 + COLON_SPREAD) + SEG_THICK/2, SEG_THICK/2, outline);
}

/*
 * For displays with a decimal point, draw the decimal point
 */
void
draw_dp(GFX_CTX *g, int x, int y, GFX_COLOR color, GFX_COLOR outline)
{
	gfx_fill_circle_at(g,
		skew_factor(DISP_HEIGHT - (SEG_THICK/2), DISP_HEIGHT) + x + SEG_THICK/2,
		y + DISP_HEIGHT - SEG_THICK/2, SEG_THICK/2, color);
	gfx_draw_circle_at(g,
		skew_factor(DISP_HEIGHT - (SEG_THICK/2), DISP_HEIGHT) + x + SEG_THICK/2,
		y + DISP_HEIGHT - SEG_THICK/2, SEG_THICK/2, outline);
}

/* Digits zero through 9, : and . */
struct digit_fb {
	int	w, h;	/* width and height */
	uint8_t *data;
} digits[12];

/*
 * This should be a data buffer which can hold 10 digits,
 * a colon, and a decimal point, note that pixels in this
 * buffer are one byte each.
 */
void digit_draw_pixel(void *, int x, int y, GFX_COLOR color);
void print_digit(int n);


/* This is the simple graphics helper function to draw into it */
void
digit_draw_pixel(void *fb, int x, int y, GFX_COLOR color)
{
	struct digit_fb *digit = (struct digit_fb *)(fb);
	uint8_t	c = (uint8_t) color.raw;
	*((digit->data + (y * digit->w)) + x) = c;
}


/*
 * This then will render a digit to the console, its good for
 * debugging.
 */
void
print_digit(int n) {
	uint8_t *row;
	int tx, ty;
	struct digit_fb *digit = &digits[n];

	printf("Digit FB size (W, H) = (%d, %d)\n", digit->w, digit->h);
	row = digit->data;
	for (ty = 0; ty < digit->h; ty++) {
		printf("%3d :", ty);
		for (tx = 0; tx < digit->w; tx++) {
			printf("%c", *row);
			row++;
		}
		printf("\n");
		row += digit->w;
	}
	/* END DEBUG CODE */
}

#define DIGIT_OUTLINE_COLOR	(GFX_COLOR){.raw=2}
#define DIGIT_BODY_COLOR	(GFX_COLOR){.raw=1}

/*
 * And this is where we will generate our digits
 * when we copy them with the DMA2D device we can
 * use the color lookup table to set the values.
 * in this case we'll use 0, 1, and 2 for background
 * foreground, and outline color.
 *
 * While the digits are kerned in the display they are
 * drawn here with a box that is DISP_WIDTH + SKEW_MAX
 * pixels wide, but DISP_HEIGHT pixels high.
 */
void
generate_digits(void)
{
	GFX_CTX	local_gfx;
	GFX_CTX	*g;
	int w, h;
	int i;

	w = DISP_WIDTH + SKEW_MAX;
	h = DISP_HEIGHT;
	for (i = 0; i < 10; i++) {
		digits[i].w = w;
		digits[i].h = h;
		digits[i].data = calloc(w * h, sizeof(uint8_t));
		printf("Digit %d, bmp allocated 0x%0X, length %d\n",
				i, digits[i].data, w*h);
		g = gfx_init(NULL, digit_draw_pixel, w, h,
					   GFX_FONT_LARGE, (void *)&digits[i]);
		gfx_fill_screen(g, C_BLANK);
		draw_digit(g, 0, 0, i, C_STAR, C_DOT);
		printf("Generated digit %d printed:\n", i);
		print_digit(i);
		if (i > 0) {
			printf("Previously generated digit:\n");
			print_digit(i-1);
		}
	}
	/* colon */
	w = SEG_THICK + SKEW_MAX;
	h = DISP_HEIGHT;
	digits[10].data = calloc(w * h, sizeof(uint8_t));
	digits[10].w = w;
	digits[10].h = h;
	g = gfx_init(&local_gfx, digit_draw_pixel, w, h,
						GFX_FONT_LARGE, (void *)&digits[10]);
		gfx_fill_screen(g, C_BLANK);
	draw_colon(g, 0, 0, C_STAR, C_DOT);

	/* decimal */
	digits[11].w = w;
	digits[11].h = h;
	digits[11].data = calloc(w * h, sizeof(uint8_t));
	g = gfx_init(&local_gfx, digit_draw_pixel, w, h,
						GFX_FONT_LARGE, (void *)&digits[11]);
		gfx_fill_screen(g, C_BLANK);
	draw_dp(g, 0, 0, C_STAR, C_DOT);
}

/*
 * This is the code for the simple DMA2D Demo
 *
 * Basically it lets you put display digits on the
 * screen manually or with the DMA2D peripheral. It
 * has various 'bling' levels, and it tracks performance
 * by measuring how long it takes to go from one frame
 * to the next.
 */
int
main(void) {

	printf("Generate digits\n");
	generate_digits();
	printf("Sample digit 6\n");
	print_digit(6);
}
