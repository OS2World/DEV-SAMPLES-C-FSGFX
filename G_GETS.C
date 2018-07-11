/* g_gets.c */
/* get a string in graphics mode */

#include "mygraph.h"

g_gets(locx,locy,temp)
int locx;
int locy;
char temp[];
{

	int i,x,y,j,k;
	int letter;
	char zchar;
	unsigned char mask;
	int spot;
	unsigned char uchar;

	i = 0;
	zchar = 0;
	letter = 0;
	if(locx < 0)
		locx = oldtextx;
	if(locy < 0)
		locy = oldtexty;
	
	while(zchar != 0x0D)
	{
	zchar = getch();

		if(zchar != 0x0D)
		{
		letter = zchar - ' ';
		temp[i++] = zchar;
			for(y = 0; y<FONTH; y++)
			{
				for(x = FONTW-1; x>=0; x--)
				{
					if(alphabet[letter][y][x])
					{
						j = locx+x;
						k = locy+y;
						g_macrodot(j,k);
					}
				}
			}
		if(locx < SCR_XMAX)
			locx += FONTW;
		else
			locx = 0;
		}
	}
	temp[i] = 0;
	oldtextx = locx;	
	oldtexty = locy;
}

