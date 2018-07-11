/* mymouse.c */
/* library of mouse routines for full screen OS/2 */


#include <stdio.h>
#include <myproc.h>
#include "mygraph.h"
#include "mymouse.h"

extern parameterid;
static int MouseWaitForEventFlag = 1;
static int MouseNoWaitForEventFlag = 0;

struct _MOUEVENTINFO MouEvent;

long showcursor = 1;
long showcursor2;
int mousex = 0;
int mousey = 0;
int mouseminx = 0;
int mouseminy = 0;
int mousemaxx = 639;
int mousemaxy = 348;
int MouseDeviceStatus = 0x0100;

int mousethreadid;
unsigned MouHandle;	/* mouse logical handle */

void msinittext(void)
{
	doscall(MouOpen(0L, &MouHandle)); 
	doscall(MouDrawPtr(MouHandle));
}

void msstatustext(void)
{
 	doscall(MouReadEventQue(&MouEvent, &MouseWaitForEventFlag, MouHandle)); 
}

static char cursor[16][9] = 
		{	{ 1,0,0,0,0,0,0,0,0 },
			{ 1,1,0,0,0,0,0,0,0 },
			{ 1,1,1,0,0,0,0,0,0 },
			{ 1,1,1,1,0,0,0,0,0 },
			{ 1,1,1,1,1,0,0,0,0 },
			{ 1,1,1,1,1,1,0,0,0 },
			{ 1,1,1,1,1,1,1,0,0 },
			{ 1,1,1,1,1,1,1,1,0 },
			{ 1,1,1,1,1,1,1,1,1 },
			{ 1,1,1,1,1,1,0,0,0 },
			{ 1,1,0,0,0,1,1,0,0 },
			{ 1,0,0,0,0,1,1,0,0 },
			{ 0,0,0,0,0,0,1,1,0 },
			{ 0,0,0,0,0,0,1,1,0 },
			{ 0,0,0,0,0,0,0,1,1 },
			{ 0,0,0,0,0,0,0,1,1 } };
static char cursormask[16][9];

void maintaincursor()
{
	int x,y;
	int spot;
	int oldmousex;
	int oldmousey;
	unsigned char uchar;
	start:

	MouEvent.col = 320;
	MouEvent.row = 175;
	oldmousex = MouEvent.col;
	oldmousey = MouEvent.row;


		for(y = 0; y<16; y++)
		{
			for(x = 8; x>=0; x--)
			{

				spot = (80 * (MouEvent.row+y)) + ((MouEvent.col+x)/8);
				uchar = 0x80 >> ((MouEvent.col+x) % 8 ); 
				if(*(pchscreen+spot) & uchar)
					cursormask[y][x] = 1;
				else
					cursormask[y][x] = 0; 
				if(cursor[y][x])
 					*(pchscreen+spot) =  *(pchscreen+spot) | uchar; 
			}
		}
		oldmousex = MouEvent.col;
		oldmousey = MouEvent.row;

	while(  1 )
	{
	g_doscall(MouReadEventQue(&MouEvent, &MouseWaitForEventFlag, MouHandle));
		if(MouEvent.col < mouseminx)
			MouEvent.col = mouseminx;
		if(MouEvent.col > mousemaxx)
			MouEvent.col = mousemaxx;
		if(MouEvent.row < mouseminy)
			MouEvent.row = mouseminy;
		if(MouEvent.row < mouseminx)
			MouEvent.row = mouseminy;

		for(y = 0; y<16; y++)
		{
			for(x = 8; x>=0; x--)
			{
				if(cursormask[y][x])
				{
					spot = (80 * (oldmousey+y)) + ((oldmousex+x)/8);
					uchar = 0x80 >> ((oldmousex+x) % 8 ); 
					*(pchscreen+spot) = *(pchscreen+spot) | uchar;
				}
				else
				{
					spot = (80 * (oldmousey+y)) + ((oldmousex+x)/8);
					uchar = 0x80 >> ((oldmousex+x) % 8 ); 
					uchar = uchar ^ 0xff;
					*(pchscreen+spot) = *(pchscreen+spot) & uchar;
				}
			}
		}

		for(y = 0; y<16; y++)
		{
			for(x = 8; x>=0; x--)
			{

				spot = (80 * (MouEvent.row+y)) + ((MouEvent.col+x)/8);
				uchar = 0x80 >> ((MouEvent.col+x) % 8 ); 
				if(*(pchscreen+spot) & uchar)
					cursormask[y][x] = 1;
				else
					cursormask[y][x] = 0; 
				if(cursor[y][x])
 					*(pchscreen+spot) =  *(pchscreen+spot) | uchar; 
			}
		}
		oldmousex = MouEvent.col;
		oldmousey = MouEvent.row;
	}
}

msinitgrph()
{
	spawnthread(&mousethreadid,maintaincursor);
	g_doscall(DosSetPrty(2,3,31,mousethreadid));
}

int mshidecur(void)
{
	;
}

int msshowcur(void)
{
	;
}

