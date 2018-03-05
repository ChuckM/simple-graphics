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
#include "test.h"

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
	print_screen();
}
