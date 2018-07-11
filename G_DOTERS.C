/* g_doters.c */
/* erase a graphics dot */

#include "mygraph.h"

int g_doters(x,y)
int x;
int y;
{
	int spot;
	unsigned char uchar;

	spot = (80 * y) + (x/8);
	uchar = (0x80 >> (x % 8)) ^ 0xff; 
	*(pchscreen+spot) = *(pchscreen+spot) & uchar; 
}
 
