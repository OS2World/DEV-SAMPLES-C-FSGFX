/* g_rdrwth.c */
/* thread for resetting graphics state across session switches */

#include "mygraph.h"
#include "mymouse.h"

void far g_rdrwth()
{
        char *oldscreen;
	unsigned int fnotify;
	oldscreen = pchscreen;
	while(1)
	{
		g_doscall(VioSavRedrawWait(0,&fnotify,0));
		if(fnotify)
		{
			g_doscall(VioSetMode(&newconfig,0));	/* set graphics mode */
			pchscreen = oldscreen; /* causes program to write to screen */
			g_restore(screenbuffer,28000);
			DosResumeThread(mousethreadid);
		}
		else
		{
			DosSuspendThread(mousethreadid);
			g_save(screenbuffer,28000); 
			pchscreen = screenbuffer; /* causes program to write to buffer */
		}
	}
}

