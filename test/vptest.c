/* vptest
 *
 * Verify the viewport code.
 *
 * At its simplest, the viewport code creates a 'window' inside the
 * display where the client can draw in its preferred co-ordinate space
 * and the graphics library translates that to pixels on the actual
 * display.
 *
 * At the moment all we can do is draw lines in the view port.
 *
 * So to verify the features here, we need to create a 'space'
 * and draw to it. And verify that the correct pixels are written
 * and that pixels that are not supposed to be written are not
 * written. And finally that we actually draw what we think we draw.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gfx.h>
#include "colors.h"

char text_screen[100*100];

void
draw_pixel(__attribute__((unused)) void *none, int x, int y, GFX_COLOR c)
{
	printf("Plot %d, %d - [%d]\n", x, y, c.raw);
	if ((x > 99) || (y > 99)) {
		fprintf(stderr, "Pixel [%d, %d] is out of bounds!\n", x, y);
		exit(1);
	}
	text_screen[y*100 + x] = (char) c.raw;
}

int
main(int argc, char *argv[])
{
	GFX_CTX	*g;
	GFX_VIEW *vp;
	memset(text_screen, ' ', sizeof(text_screen));

	g = gfx_init(NULL, draw_pixel, 100, 100, GFX_FONT_TINY, NULL);
	vp = gfx_viewport(NULL, g, 25, 25, 50, 50, -1.0, -1.0, 1.0, 1.0);
	vp_plot(vp, -.25, -.25, .25, .25, C_STAR);
	printf("Out of bounds test\n");
	vp_plot(vp, 0.5, 0, 1.5, 0, C_PLUS);
	printf("Axis flipped test\n");
	vp_plot(vp, 0, 0, 0, 1., C_ONE);
}
