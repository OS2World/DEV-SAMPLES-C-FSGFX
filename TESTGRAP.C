/* testgrap.c */
/* get basic graphics and mouse happening */
#include <stdio.h>
#include "mygraph.h" 
#include "mymouse.h" 
#include "myproc.h"

struct rect outtahere;
struct rect findafile;
main()
{
	int button;
	struct point pt;
	char temp[80];
	char temp1[80];
	g_setgph();
	msinitgrph();
	g_puts(0,0,"Hey and Hello. Funny font with 640 x 350 OS/2 "
		"full screen graphics");
	g_puts(0,16,"Direct screen writing is used, so non standard"
		" video modes can be supported.");
	g_puts(0,32,"Threads are created for session switch, mode change"
		" and mouse drawing");

		outtahere = g_drwbtn(100,100,"exit","");
		findafile = g_drwbtn(400,320,"Find a File","");
	g_line(50,50,200,100);
	for( ; ; )
	{
		while(button == 0)
			msstatus(button,pt);
		if(ptinrect(pt,outtahere))
			exit(0);
		else if(ptinrect(pt,findafile))
			g_fselect("*",temp);
		button = 0;
	
	}
	g_rmode();
}


