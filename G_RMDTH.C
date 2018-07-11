/* g_rmdth.c */
/* restore mode thread for resetting mode after vio pop ups, or with OS/2
1.0 */

#include "mygraph.h"

void far g_rmdth()
{
	unsigned int fnotify;
	fnotify = 0;
	while(1)
	{
		g_doscall(VioModeWait(0,&fnotify,0));
		g_doscall(VioSetMode(&newconfig,0));	/* set graphics mode */
	}
}

