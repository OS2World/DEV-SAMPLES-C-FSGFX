a history of using IBM OS/2 without the $$$$ "toolkit", just the 
os/2 1.0 tools included with MSC 5.1 & MASM 5.1. Look what it has
led to.
 
this thing assumes too much, but will work on most OS/2 setups with
a VGA and mouse installed. part of the reasoning behind it was to
build graphics apps under OS/2 1.0 on a hercules monochrome monitor,
but things change so quickly. 

released to the world to release me from such a headache. 
others might find the session switch handling of interest, as well
as some of the graphics code, which was distilled from many
public domain sources. remember the freeware motto -
so show me yours!

/* */
extern int returnvalue;
int returnvalue;
#define doscall(x) if(returnvalue = x) { printf(#x " = %d\n",returnvalue); getch(); exit(returnvalue); }
/* */

add these to your os2.h file for an easier time, and these
files are set up to "include the world" add #ifdef's to your desire

