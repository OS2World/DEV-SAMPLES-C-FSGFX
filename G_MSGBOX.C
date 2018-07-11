/* g_msgbox.c */
/* draw a box with text messages */

#include "mygraph.h"

g_msgbox(int x,int y, int width, int height, char message1[], char message2[])
{
	struct rect outer;
	struct rect inner;

	g_puts(x + (width/2) - ((strlen(message1)+
		strlen(message2))*8/2), y - 10,	message1);
	g_puts(-1, -1, message2);

	outer.pt1.x = x;
	outer.pt1.y = y;
	outer.pt2.x = x+width;
	outer.pt2.y = y+height;
	g_rect(outer);

	inner.pt1.x = x+2;
	inner.pt1.y = y+1;
 	inner.pt2.x = inner.pt1.x+width	- 4;
	inner.pt2.y = inner.pt1.y+height - 2;
	g_rect(inner);
}

