/* testgrap.h */
extern struct _VIOCONFIGINFO vioconfig;
extern struct _VIOPHYSBUF  viopbBuf;	
extern struct _VIOMODEINFO oldconfig;
extern struct _VIOMODEINFO newconfig;
extern struct _MOUEVENTINFO MouEvent;
extern struct _SCALEFACT  mousc;
extern struct _PTRSHAPE mousepointershape;
extern char mousepointerbuffer[1];
extern unsigned MouHandle;
extern int MouseWaitForEventFlag;
extern int MouseNoWaitForEventFlag;
extern int MouseDeviceStatus;
extern	unsigned int cblvb;
extern	char *pchscreen;
extern	int fstatus;
extern	int fsdevstatus;
extern	int redraw_mode;

