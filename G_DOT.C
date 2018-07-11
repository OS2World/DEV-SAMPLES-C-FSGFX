/* g_dot.c */
/* put a graphics dot */

#include "mygraph.h"

int g_dot(x,y)
int x;
int y;
{
	int spot;
	unsigned char uchar;
	spot = (80 * y) + (x/8);
	uchar = 0x80 >> (x % 8 ); 

	if(redrawmode == OR)
		*(pchscreen+spot) =	*(pchscreen+spot) | uchar; 
	else if(redrawmode == XOR)
		*(pchscreen+spot) =	*(pchscreen+spot) ^ uchar; 
	else if(redrawmode == AND)
		*(pchscreen+spot) =	*(pchscreen+spot) & uchar; 
}

