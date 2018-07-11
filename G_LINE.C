/* g_line.c */
/* draws a graphic line */

#include "mygraph.h"

int g_line(int startx, int starty, int endx, int endy)
{
	int spot;
	unsigned char uchar;

	register int t, distance;
	int xerr=0, yerr=0, delta_x, delta_y;
	int incx, incy;

	delta_x=endx-startx;
	delta_y=endy-starty;

	if(delta_x>0) 
		incx=1;
	else if(delta_x==0) 
		incx = 0;
	else incx = -1;

	if (delta_y>0) 
		incy=1;
	else if(delta_y == 0) 
		incy = 0;
	else 
		incy = -1;

	delta_x=abs(delta_x);
	delta_y=abs(delta_y);

	if(delta_x>delta_y)
		distance=delta_x;
	else 
		distance = delta_y;

	for(t=0; t<=distance+1; t++)
	{
		g_macrodot(startx,starty);
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance) 
		{
			xerr-=distance;
			startx+=incx;
		}
		if(yerr>distance)
		{
			yerr -= distance;
			starty += incy;
		}
	}
}

