/* g_hfader.c */
/* draw a horizontal fader */

#include "mygraph.h"

g_hfader(int x,int y,int fvalue)
{
	struct rect knob;
	struct rect body;
	knob.pt1.x = x + 2 + fvalue;
	knob.pt1.y = y + 2;
	knob.pt2.x = knob.pt1.x + 20;
	knob.pt2.y = knob.pt1.y + 6;
	body.pt1.x = x;
	body.pt1.y = y;
	body.pt2.x = body.pt1.x + 152;
	body.pt2.y = body.pt1.y + 10;
	g_rect(knob);
	g_rect(body);
}

