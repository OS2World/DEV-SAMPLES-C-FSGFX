/* mygraph.h */

#include <os2.h>

#define 	SCR_XMAX	640
#define		SCR_YMAX	350
#define		FONTH		8	/* font height */
#define		FONTW		8	/* font width */

#define g_doscall(x) if(returnvalue = x) { g_rmode(); printf(#x " = %d\n",returnvalue); getch(); exit(returnvalue); }
#define g_mode(x) redrawmode = x;
#define g_save(a,b) memcpy(a,pchscreen,b);
#define g_restore(a,b) memcpy(pchscreen,a,b);
#define g_solid()
#define	OR		0
#define XOR		1
#define AND		2

struct point {
		int x;
		int y;
	};

struct rect {
		struct point pt1;
		struct point pt2;
	};

/* uses rect structure to create a rectangle on screen */
/* by drawing lines */
#define g_rect(s)	 	g_line(s.pt1.x,s.pt1.y,s.pt1.x,s.pt2.y); \
				g_line(s.pt2.x,s.pt1.y,s.pt2.x,s.pt2.y); \
				g_line(s.pt1.x,s.pt1.y,s.pt2.x,s.pt1.y); \
				g_line(s.pt1.x,s.pt2.y,s.pt2.x,s.pt2.y); 

extern struct _VIOPHYSBUF viopbBuf;
extern struct _VIOMODEINFO oldconfig;
extern struct _VIOMODEINFO newconfig;

extern unsigned char alphabet[128][8][8];
extern int oldtextx;	/* external variables, local to this module */
extern int oldtexty;

extern int g_fselect(char searchpath[],char filename[]);
extern int g_clrscn(void);
extern char *screenbuffer;
extern char *pchscreen;
extern int ptinrect(struct point pt, struct rect r);
extern int redrawmode;
extern int returnvalue;
extern int g_setgph( void );
extern int g_rmode( void );
extern int g_dot(int x, int y);
extern int g_erase(int x, int y);
extern int g_line(int startx, int starty, int endx, int endy);
extern int g_puts(int locx,int locy,char temp[]);
extern int g_gets(int locx,int locy,char temp[]);

extern int g_msgbox(int x,int y, int width, int height, char message1[], char message2[]);
extern struct rect g_drwbtn(int x,int y, char message1[], char message2[]);
extern int g_hfader(int x,int y,int fvalue);
extern int g_vfader(int x, int y, int fvalue);


extern void g_rdrwth(void);
extern void g_rmdth(void);


#define g_macrodot(x,y) spot = (80 * y) + (x/8);		\
	uchar = 0x80 >> (x % 8 );				\
	if(redrawmode == OR)					\
		*(pchscreen+spot) = *(pchscreen+spot) | uchar;	\
	else if(redrawmode == XOR)				\
		*(pchscreen+spot) = *(pchscreen+spot) ^ uchar;	\
	else if(redrawmode == AND)				\
		*(pchscreen+spot) = *(pchscreen+spot) & uchar;	

#define g_macrodoterase(x,y) spot = (80 * y) + (x/8);		\
	uchar = (0x80 >> (x % 8 )) ^ 0xff;			\
	*(pchscreen+spot) = *(pchscreen+spot) & uchar; 

