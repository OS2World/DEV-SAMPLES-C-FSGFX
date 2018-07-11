/* g_puts.c */
/* put a string in graphics mode */

#include "mygraph.h"

g_puts(locx,locy,temp)
int locx;
int locy;
char temp[];
{
	register int x;
	register int y;
	int i,j,k;
	int letter;
	char zchar;
	unsigned char mask;
	int spot;
	unsigned char uchar;

	i = 0;

	if(locx < 0)
		locx = oldtextx;
	if(locy < 0)
		locy = oldtexty;
	while(temp[i] != 0)
	{
		letter = temp[i++] - ' ';

		for(y = 0; y<FONTH; y++)
		{
			for(x = FONTW-1; x>=0; x--)
			{
				j = locx+x;
				k = locy+y;	
				if(alphabet[letter][y][x])
				{
					g_macrodot(j,k);
				}
				else
				{
					g_macrodoterase(j,k);
				}
			}
		}

		if(locx < SCR_XMAX)
			locx += FONTW;
		else
			locx = 0;
	}
	oldtextx = locx;	
	oldtexty = locy;
}


