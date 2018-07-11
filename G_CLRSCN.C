/* g_clrscn.c */
/* clear the graphics screen */

#include "mygraph.h"

int g_clrscn(void)
{
	register unsigned int i;
	for(i = 0; i< 28000; i++)
		pchscreen[i] = 0x00;
}

