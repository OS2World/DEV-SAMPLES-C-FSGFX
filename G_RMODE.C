/* g_rmode.c */
/* restore initial graphics settings, used after calling
setgraph */

#include "mygraph.h"

int g_rmode(void)
{
	int returnvalue;
	g_doscall(VioSetMode(&oldconfig,0));
}

