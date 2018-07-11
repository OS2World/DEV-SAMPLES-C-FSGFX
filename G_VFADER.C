/* g_vfader.c */
/* draw vertical fader */

#include "mygraph.h"

int g_vfader(int x, int y, int fvalue)
{
	struct rect knob;
	struct rect body;

	g_solid();/* set line type */
	g_mode(XOR);
	knob.pt1.x = x + 2;
	knob.pt1.y = y + 152 - fvalue - 20 - 2;
	knob.pt2.x = knob.pt1.x+8;
	knob.pt2.y = knob.pt1.y+20;
	g_rect(knob);
	g_mode(OR);

	body.pt1.x = x;
	body.pt1.y = y;
	body.pt2.x = x+12;
	body.pt2.y = y+152;
	g_rect(body);
}

