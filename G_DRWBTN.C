/* g_drwbutn.c */
/* draw a button with a text message in the middle */

#include <string.h>
#include "mygraph.h"

struct rect g_drwbtn(int x,int y, char message1[], char message2[])
{
	struct rect inner;
	struct rect outer;
	int width;
	char temp[80];

	strcpy(temp, message1);
	strcat(temp,message2);

	width = (strlen(temp)*8)+16;
	g_puts(x + 8, y + (14 / 3),	temp);

	outer.pt1.x = x;
	outer.pt1.y = y;
	outer.pt2.x = x + width;
	outer.pt2.y = y + 14; /* 14 = height */
	g_rect(outer);

	inner.pt1.x = x + 2;
	inner.pt1.y = y + 1;
	inner.pt2.x = inner.pt1.x + width-4;
	inner.pt2.y = inner.pt1.y + 14;
	g_rect(inner);
	return(inner);
}

