/*  g_fselct.c */
/*  simple file selector box    */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#include "mygraph.h"
#include "mymouse.h"

#define BSPACE  20   
#define BHEIGHT 16
#define FXSPOT 27*8
#define FYSPOT 17*8
#define FHSIZE 16*8
#define MAXFILES 5
#define FVSIZE MAXFILES*8 +6

static unsigned char sp[16][16] =
	{  { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
	{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
	{ 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0 },
	{ 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 },
	{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 } };
/*   { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15 }*/


static struct _FILEFINDBUF dirlist[200];
#define msstatus(b,x,y) if(MouEvent.fs & 0x06) b=1; else b=0; x = MouEvent.col; y = MouEvent.row; 
int g_fselect(char searchpath[],char filename[])
{
	register int x,y;
	int spot;
	unsigned char uchar;
	int filecount;
	int done;
	int a, b, c,z,bstat, offset, filenumber, selected;
	char blank[13];

	int i = 0;
	unsigned shan = -1;
	int scnt = 1;

	strcpy(blank,"             ");
	mshidecur();
	msstatus(bstat,x,y);
	mshbounds(x,x);
	msvbounds(y,y);
	if(!DosFindFirst(searchpath,&shan,_A_NORMAL,&dirlist[i],sizeof(dirlist[i]),&scnt,0L))
	{
		do i++;
		while(DosFindNext(shan,&dirlist[i],sizeof(dirlist[i]),&scnt)==0);

	}	
	DosFindClose(shan);
	filecount = i;

	for(x = 0; x<FHSIZE; x++)
	for(y = 0; y<FVSIZE+24; y++)
		{
		g_macrodoterase((FXSPOT+x),(FYSPOT-8+y));
		}

	g_msgbox(FXSPOT,FYSPOT,FHSIZE,FVSIZE,searchpath,"");
	g_line(FXSPOT+FHSIZE-22,FYSPOT,FXSPOT+FHSIZE-22,FYSPOT+FVSIZE);

	for (x = 0; x<16; x++)
		for (y=0; y<16; y++)
			if(sp[y][x] == 1)
				g_dot(FXSPOT+FHSIZE-20+x,y+FYSPOT+FVSIZE-18);

	for (x = 0; x<16; x++)
		for (y=0; y<16; y++)
			if(sp[y][x] == 1)
				g_dot(FXSPOT+FHSIZE-20+x,-y+FYSPOT+18);

	g_drwbtn(FXSPOT,FYSPOT+FVSIZE,"Ok","");
	g_drwbtn(FXSPOT+FHSIZE-8*8,FYSPOT+FVSIZE,"Cancel","");

	msvbounds(0,SCR_YMAX-16);
	mshbounds(0,SCR_XMAX-16);
	for(a = 0; a < MAXFILES; a++)
		g_puts(FXSPOT+8,(8*a)+FYSPOT+4,dirlist[a].achName);
	msshowcur();
	offset = 0;
	done = 0;
	selected = 0;
	while( done >= 0)
	{
		bstat = 0;
		msvbounds(0,342);
		mshbounds(0,640);
		while(bstat == 0 && !kbhit())
			msstatus(bstat,x,y);
		if(kbhit())
			return(0);
		if(x>FXSPOT && y > FYSPOT +FVSIZE && x < FXSPOT + 8*8
			&& y < FYSPOT + FVSIZE+BHEIGHT)
		done = -1;

		if(x>FXSPOT+FHSIZE-8*8 && y > FYSPOT +FVSIZE && x < FXSPOT + FHSIZE
			&& y < FYSPOT + FVSIZE+BHEIGHT)
		{
			done = -1;
			strcpy(filename,"");
			selected = 0;
		}

		if(x>FXSPOT && y > FYSPOT && x < FXSPOT + FHSIZE-24
			&& y < FYSPOT + FVSIZE)
		{
			a = (y-FYSPOT-4) / 8;
			g_puts(FXSPOT,FYSPOT+FVSIZE+18,blank);
			g_puts(FXSPOT,FYSPOT+FVSIZE+18,dirlist[a+offset].achName);
			strcpy(filename,dirlist[a+offset].achName);
			selected = -1;
		}

		if(x>FXSPOT+FHSIZE-22 && y > FYSPOT && x < FXSPOT + FHSIZE
			&& y < FYSPOT + 20)
		{
			msvbounds(y,y);
			mshbounds(x,x);
			while(bstat == 1)
			{
				offset--;
				if (offset < 0)
					offset = 0;
				for(a = 0; a < MAXFILES; a++)
				{
					g_puts(FXSPOT+8,(8*a)+FYSPOT+4,dirlist[a+offset].achName);
					c = strlen(dirlist[a+offset].achName);
					if(c< 12)
						for(x = 0; x <12-c; x++)
							g_puts(-1,-1," ");
				}
			msstatus(bstat, x, y);
			}
		}

		if(x>FXSPOT+FHSIZE-22 && y > FYSPOT+FVSIZE-20 && x < FXSPOT + FHSIZE
			&& y < FYSPOT+FVSIZE)
		{
			msvbounds(y,y);
			mshbounds(x,x);
			while(bstat == 1)
			{
				if(filenumber > MAXFILES)
					offset++;

				if(offset > filenumber-MAXFILES && filenumber >= MAXFILES)
					offset = filenumber-MAXFILES;

				for(a = 0; a < MAXFILES; a++)
				{
					g_puts(FXSPOT+8,(8*a)+FYSPOT+4,dirlist[a+offset].achName);
					c = strlen(dirlist[a+offset].achName);
					if(c< 12)
					for(x = 0; x <12-c; x++)
					g_puts(-1,-1," ");
				}
			msstatus(bstat, x, y);
			}
		}

		while(bstat == 1)
		msstatus(bstat,x,y);
	}
	if (selected < 0)
		return -1;
	else
		return 0;
}
