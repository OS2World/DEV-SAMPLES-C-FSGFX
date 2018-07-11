/* edit.c companion file to sampedit.c */
/* 6/30/89 */
#define _STAT_DEFINED 1

#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "fcntl.h"
#include "string.h"
#include "dos.h"
#include "io.h"
#include "fcntl.h"
#include "conio.h"
#include "mpu\mpu.h"
#include "hsa\hsa_gr.h"
#include "sys\stat.h"
#include "math.h"

/* monitor definitions */

#define		COL		8
#define		ROW		8



/* offsets to values in sample stored at unsigned char *pointer */

#define PRO_OFF		0
#define HEAD_OFF	19
#define BLOCK_SIZE	122



#define BUFFSIZE 8704
#define SETMODE 0
#define VIDEO	0x10
#define MASK	 0xCF

union REGS regs;
unsigned char buff[BUFFSIZE];

extern void error(int errornumber);

extern int sampbuff;

extern struct samp {
	unsigned char header[19];
        unsigned char programs[320];
        long lof,period,words,loopstart,loopend,blocks;
} samps[16];





convert(pointer,sample,sampbuff,blocks)
unsigned char huge *pointer;
unsigned int far * *sample;
int sampbuff;
long blocks;
{
unsigned int a,b;
unsigned long part, offset,i,x,y;

for(a = 0; a<HEAD_OFF; a++)
	samps[sampbuff].header[a] = pointer[a];

offset = 0;
a = 0;
do
{
part = (offset * 122) + HEAD_OFF;
	for(i = part+1; i<part+120; i++)
		{
	x = pointer[i++] << 5;
	y = pointer[i] >>2;
	sample[sampbuff][a++] = x | y;
/*	i++;
	a++;*/
		}
	offset++;
}while(offset<blocks);

}

loadsamp(pointer,zz,lof,sampbuff)
unsigned char huge *pointer;
char zz[];
long *lof;
int sampbuff;
{
long i;
long a;
int bytes,inhandle;

	if( (inhandle = open(zz, O_RDONLY | O_BINARY)) < 0) /* open that big file*/
		{
		error(1);
		return(0);
		}
i = 0;
	while( (bytes = read(inhandle, buff, BUFFSIZE)) > 0)
		{
		for (a = 0; a<bytes; a++)
			{		/*	load that big file into far mem*/
			pointer[i] = buff[a];
			i++;
			}
		}
	close(inhandle);

for(a = 0; a<HEAD_OFF; a++)
	samps[sampbuff].header[a] = pointer[a];

	*lof = i;
}

deconvert(pointer, sample, sampbuff, blocks)
unsigned char huge *pointer;
unsigned int far * *sample;
int sampbuff;
long blocks;
{
	int a,b, c,h, x,y, t ;
	unsigned long i;
	unsigned long u;
	unsigned long part;
	unsigned long offset;

for(a = 0; a<HEAD_OFF; a++)
	pointer[a] = samps[sampbuff].header[a];

offset = 0;
a = 0;
do
{
part = (offset * 122) + HEAD_OFF;
	for(i = 1; i<120; i++)
		{
	pointer[i+part] = (sample[sampbuff][a] >> 5) & 0x7F;	/* zaphod */
	pointer[i+part+1] = (sample[sampbuff][a] << 2) & 0x7F;
	a++;
	i++;
		}
	offset++;
}while(offset<blocks && !kbhit());
}


redo_checksum(pointer,blocks)
unsigned char huge *pointer;
long blocks;
{
unsigned long i, part, offset, checksum;
offset = 0;
checksum = 0;
do
{
part = (offset * 122) + HEAD_OFF;
for(i=part+1; i<part+121; i++)
	checksum = pointer[i] ^ checksum;
	pointer[part+121] = checksum;
	checksum = 0;
	offset++;
}while(offset<blocks);
}






savesamp(pointer,zz1,lof)
unsigned char huge *pointer;
char zz1[];
long lof;
{
	register long i;
	register long bytes;
	int outhandle;

	if( (outhandle = open(zz1,
		O_CREAT | O_WRONLY | O_BINARY | S_IWRITE)) < 0)
		{
        	error(1);
		return(0);
		}
	bytes = 0;
	for(i = 0; i<lof; i++)
		{
		if(bytes > BUFFSIZE)
			{
			write(outhandle,buff,bytes);
			bytes = 0;
			}
		buff[bytes] = pointer[i];
		bytes++;
		}
	if(bytes > 0)
		write(outhandle,buff,bytes);
	close(outhandle);
}

videomode(mode)
int mode;
{
	unsigned int far *farptr;
	unsigned char ch;
	int a;
	unsigned char vidbits;
	if (mode == 7)
		vidbits = 0x30;
	else
		vidbits = 0x10;
	farptr = (unsigned int far *) 0x410;
	ch = *(farptr);
	ch = ch & MASK;
	for (a = 0; a<10000; a++);
	*(farptr) = ch | vidbits;
	regs.h.al = (char) mode;
	regs.h.ah = SETMODE;
	int86(VIDEO, &regs, &regs);
}
long tbconvert(index, sampbuff)
int index;
int sampbuff;
{
	long sampsize;
	long samphold;
	double doubler;
        samphold = samps[sampbuff].header[index+2];
	sampsize = samphold << 14;
	samphold = samps[sampbuff].header[index+1];
	sampsize=sampsize | (samphold << 7);
	sampsize=sampsize | samps[sampbuff].header[index];
	return(sampsize);
}
void tbdeconvert(tb,index,sampbuff)
long tb;
long index;
int sampbuff;
{
	index = index + PRO_OFF;
	samps[sampbuff].header[index] = tb & 0x7F;
	index++;
	samps[sampbuff].header[index] = (tb >> 7) & 0x7F;
	index++;
	samps[sampbuff].header[index] = (tb >> 14) & 0x7F;

}

int deconvert_send(sample,sampbuff,blocks)
unsigned int far * *sample;
int sampbuff;
long blocks;
{
	unsigned char bnum, byte, unchar, checksum;
	unsigned long a,i,index;
	int offset;
	char temp[80];
	offset = 0;

	if(!setmpu(CMD_RESET))
		{
		g_str_h(0,0,"MPU error");
		error(2);
		return(-1);	
		}
	else
		setmpu(CMD_UART);


	while(rxmidi(&byte));/*clear stray bytes*/

	for (i = PRO_OFF; i < PRO_OFF + HEAD_OFF; i++ ) /* send the header */
		{ 
		if(i == PRO_OFF+3) 
			txmidi(0);	
		else 
		/*	txmidi(pointer[i]); */
			txmidi(samps[sampbuff].header[i]);
		}
	bnum = 0;
	do 
		{	
		getshake();
		txmidi(bnum++);
		if (bnum == 128)
			bnum = 0;
		checksum = 0;
		index = offset * 60;
		for(a = index; a<60+index; a++)
			{
			unchar = (sample[sampbuff][a] >> 5) & 0x7F;
			checksum = unchar ^ checksum;
			txmidi(unchar);
			unchar = (sample[sampbuff][a] << 2) & 0x7F;
			checksum = unchar ^ checksum;
			txmidi(unchar);
			}
		txmidi(checksum);
		g_int(offset+(COL*15),ROW-4);
		}while(offset++ < blocks);

	txmidi(247);

	if(!setmpu(CMD_RESET))
		return(-1);
	else
		return(0);
}
getshake()
{
	int i;
	unsigned char byte;
	char temp[80];
	do
	{
		if(!rxmidi(&byte))
			{
			error(2);
			return(0);
			}
	}while( byte !=240 && !kbhit());

	if(kbhit())
		{
		getch();
		return(-1);
		}

	for(i = 0; i<3; i++)
		{
		if(!rxmidi(&byte))
			{
			error(2);
			return(0);
			}
		if(i==1 && byte != 127)
			{
			g_str_h(0,0,"                                                                      ");
			g_str_h(0,0, itoa(byte,temp,10) );
			}	
		}
}

how_many_blocks(words,blocks)
long words;
long *blocks;
{
	double doubler;
	doubler = words;
		if((words / 60) - (doubler/60) < 0)
			*blocks = words / 60 +1;
		else
			*blocks = (words / 60);
}
