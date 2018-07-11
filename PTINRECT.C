/* ptinrect.c */
/* is the point in the rectangle? */

#include "mygraph.h"

int ptinrect(struct point pt,struct rect r)
{
return		pt.x >= r.pt1.x && pt.x < r.pt2.x	&&
		pt.y >= r.pt1.y && pt.y < r.pt2.y;
}

