#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <myproc.h> 

int spawnthread(int *threadid, void (*fxn)(void))
{
	char *threadstack;
	if((threadstack = malloc(64000)) == NULL)
	{
		printf("Unable to allocate stacks");
		return(2);
	}
	return(DosCreateThread(fxn,threadid,
		threadstack+64001));
}
