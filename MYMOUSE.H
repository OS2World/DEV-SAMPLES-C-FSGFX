/* mymouse.h */
/* include file for mouse functions */

extern int mousethreadid;
extern int mouseminx;
extern int mousemaxx;
extern int mouseminy;
extern int mousemaxy;
extern  char *pchscreen;
extern  int returnvalue;
extern  int redrawmode;
extern  long showcursor;
extern  void maintaincursor(void );

extern struct _MOUEVENTINFO MouEvent;
extern unsigned MouHandle;
extern int MouseDeviceStatus;
extern void msinittext(void);
extern void msstatustext(void);

#define msinitgraphics() g_doscall(MouOpen(0L, &MouHandle)); g_doscall(MouSetDevStatus(&MouseDeviceStatus,MouHandle)); 
#define msstatusgraphics() g_doscall(MouReadEventQue(&MouEvent, &MouseWaitForEventFlag, MouHandle)); 

#define msstatus(b,pt) if(MouEvent.fs & 0x06) b=1; else b=0; pt.x = MouEvent.col; pt.y = MouEvent.row; 
#define	mshbounds(a,b) mouseminx = a; mousemaxx = b;
#define	msvbounds(a,b) mouseminy = a; mousemaxy = b;
extern int mshidecur(void);
extern int msshowcur(void);


