/* buttons.c */
/* buttons and fileio from mks7.c */

#include <stdio.h>
#include "mygraph.h"
#include "mymouse.h"
#include "mymididl.h"
#include "mks7.h"


int channel = 0;
int patch = 98;

char disk[3][15] = { "Exit",
			"Load",
			"Save" };
int instrument = 1;
unsigned char switchbyte[3] = {0x60,0x3C,0x17};
char description[] = "default patch";
struct parm pm[] =
	{ { "Rate", 127 },/*LFO*/
	{ "Delay", 54 },/*LFO*/
	{ "Modulation",  0 },/*DCO LFO*/
	{ "Pulse Width",  0 },/*DCO PWM*/
	{ "Cutoff frequency", 64 },/*VCF*/
	{ "Resonance", 64 },/*VCF*/
	{ "Envelope", 33 },/*VCF ENV*/
	{ "Modulation", 64 },/*VCF */
	{ "Keyboard Follow ", 64 },/*VCF*/
	{ "VCA Level", 64 },/*VCA LEVEL*/
	{ "Attack", 64 },/*ENV*/
	{ "Decay", 64 },/*ENV*/
	{ "Sustain", 64 },/*ENV*/
	{ "Release", 64 },/*ENV*/
	{ "SUB Oscillator", 64 } };/*ENV*/

struct switches sw[] =
	{ { "Velocity",  1, 0x40, "on "	},/*VCA*/
	{ "Velocity",  1, 0x20, "on "	},/*VCF*/
	{ "Chorus",	0, 0x00, "on "	},/**/
	{ "Sawtooth",  0, 0x00, "off"	},/**/
	{ "Pulse",	0, 0x00, "off"	},/**/
	{ "Range",	8, 0x02, "8' "	},/**/
	{ "Noise",	0, 0x00, "off"	},/**/
	{ "High Pass", 1, 0x10, "off"	},/**/
	{ "Source",	0, 0x00, "ENV "  },/*VCA*/
	{ "Inversion", 0, 0x00, "PLUS " },/*ENV*/
	{ "Source",	0, 0x00, "LFO"	},/*PWM*/
	{ "Whole",	0, 0x00, "off"	},/**/
	{ "Hold",	0, 0x00, "off"	},/**/
	{ "Chord",	3, 0x00, "3 "	},/**/
	{ "Melody",	1, 0x00, "1 "	},/**/
	{ "Bass",	2, 0x00, "2 "	} };/**/



draw_knob(ptspot, fspace, knumber, kpos)
struct point ptspot;
int fspace;
int knumber;
int kpos;
{
	struct point kspot;
	int kwidth = 20;
	int kheight = 6;

	kspot.x = ptspot.x + 10 - 8 + pm[knumber].value;
	kspot.y = ptspot.y + 2 + (fspace * kpos);

	g_mode(XOR);
	g_line(kspot.x,kspot.y,kspot.x+kwidth,kspot.y);		/* draw knob */
	g_line(kspot.x+kwidth,kspot.y,kspot.x+kwidth,kspot.y+kheight);
	g_line(kspot.x+kwidth,kspot.y+kheight,kspot.x,kspot.y+kheight);
	g_line(kspot.x,kspot.y+kheight,kspot.x,kspot.y);
	g_mode(OR);
}

erase_knob(ptspot, fspace, knumber, kpos)
struct point ptspot;
int fspace;
int knumber;
int kpos;
{
	int kx;
	int ky;
	int kwidth = 20;
	int kheight = 6;

	kx = ptspot.x + 10 - 8 + pm[knumber].value;
	ky = ptspot.y + 2 + (fspace * kpos);

	g_mode(XOR);	/* XOR the old one */
	g_line(kx,ky,kx+kwidth,ky);		/* erase know */
	g_line(kx+kwidth,ky,kx+kwidth,ky+kheight);
	g_line(kx+kwidth,ky+kheight,kx,ky+kheight);
	g_line(kx,ky+kheight,kx,ky);
	g_mode(OR);
}

struct point d0pt = { DXSPOT+(0*DSPACE), DYSPOT };
struct point d1pt = { DXSPOT+(1*DSPACE), DYSPOT };
struct point d2pt = { DXSPOT+(2*DSPACE), DYSPOT };
struct rect d0rect;
struct rect d1rect;
struct rect d2rect;

struct point i0pt = { IXSPOT+(0*ISPACE), IYSPOT };
struct point i1pt = { IXSPOT+(1*ISPACE), IYSPOT };
struct point i2pt = { IXSPOT+(2*ISPACE), IYSPOT };
struct rect i0rect;
struct rect i1rect;
struct rect i2rect;

struct point omix0pt = { OMIXFXSPOT,OMIXFYSPOT+1*OMIXFSPACE };
struct point omix1pt = { OMIXFXSPOT,OMIXFYSPOT+2*OMIXFSPACE };
struct point omix2pt = { OMIXFXSPOT,OMIXFYSPOT+3*OMIXFSPACE };
struct point omix3pt = { OMIXFXSPOT,OMIXFYSPOT+4*OMIXFSPACE };
struct rect omix0rect;
struct rect omix1rect;
struct rect omix2rect;
struct rect omix3rect;

struct point smix0pt =  { SMIXFXSPOT,SMIXFYSPOT+1*SMIXFSPACE };
struct point smix1pt =  { SMIXFXSPOT,SMIXFYSPOT+2*SMIXFSPACE };
struct point smix2pt =  { SMIXFXSPOT,SMIXFYSPOT+3*SMIXFSPACE };
struct point dcof0pt =  { DCOFXSPOT, DCOFYSPOT+DCOFSPACE*2 };
struct point dcof1pt =  { DCOFXSPOT+BWIDTH10,DCOFYSPOT+DCOFSPACE*2 };
struct point vcffpt =  { VCFFXSPOT, VCFFYSPOT+5*VCFFSPACE };
struct point envfpt =  { ENVFXSPOT, ENVFYSPOT+4*ENVFSPACE };
struct rect smix0rect;
struct rect smix1rect;
struct rect smix2rect;
struct rect dcof0rect;
struct rect dcof1rect;
struct rect vcffrect;
struct rect envfrect;


menu()
{
	int a;
	char temp[80];
	g_clrscn();
	g_puts(0,0,"MKS-7 Editor Librarian, WITH GRAPHIC ENVELOPE!");

	d0rect = g_drwbtn(DXSPOT+(0*DSPACE),DYSPOT,disk[0],"");
	d1rect = g_drwbtn(DXSPOT+(1*DSPACE),DYSPOT,disk[1],"");
	d2rect = g_drwbtn(DXSPOT+(2*DSPACE),DYSPOT,disk[2],"");
		i0rect = g_drwbtn(IXSPOT+(0*ISPACE),IYSPOT,
	sw[13+instrument].name, sw[13+instrument].status);
	i1rect = g_drwbtn(IXSPOT+(1*ISPACE),IYSPOT,
		sw[11].name, sw[11].status);
	i2rect = g_drwbtn(IXSPOT+(2*ISPACE),IYSPOT,
		sw[12].name, sw[12].status);

	for(a = 0; a < 2; a++)
	{
	g_puts(LFOFXSPOT, LFOFYSPOT - 8 + (a * LFOFSPACE), pm[a].name);
	g_hfader(LFOFXSPOT, LFOFYSPOT+(a*LFOFSPACE) , pm[a].value);
	g_msgbox(LFOFXSPOT-16, LFOFYSPOT-12, 23*8, 2*LFOFSPACE+8,
	"Low Frequency Oscillator", "");
	}
/* draw mix faders */

	g_puts(OMIXFXSPOT, OMIXFYSPOT - 8, pm[9].name);
	g_hfader(OMIXFXSPOT, OMIXFYSPOT, pm[9].value);
	g_msgbox(OMIXFXSPOT-16, OMIXFYSPOT-12, 23*8, 5*OMIXFSPACE+8,
	"Output Mix","");
	omix0rect = g_drwbtn(OMIXFXSPOT,OMIXFYSPOT+1*OMIXFSPACE,
		sw[0].name, sw[0].status);
	omix1rect = g_drwbtn(OMIXFXSPOT,OMIXFYSPOT+2*OMIXFSPACE,
		sw[8].name, sw[8].status);
	omix2rect = g_drwbtn(OMIXFXSPOT,OMIXFYSPOT+3*OMIXFSPACE,
		sw[7].name, sw[7].status);
	omix3rect = g_drwbtn(OMIXFXSPOT,OMIXFYSPOT+4*OMIXFSPACE,
		sw[2].name, sw[2].status);

	g_puts(SMIXFXSPOT, SMIXFYSPOT - 8, pm[14].name);
	g_hfader(SMIXFXSPOT, SMIXFYSPOT, pm[14].value);
	g_msgbox(SMIXFXSPOT-16, SMIXFYSPOT-12, 23*8, 4*SMIXFSPACE+8,
	"Source Mix","");
	smix0rect = g_drwbtn(SMIXFXSPOT,SMIXFYSPOT+1*SMIXFSPACE,
		sw[4].name, sw[4].status);
	smix1rect = g_drwbtn(SMIXFXSPOT,SMIXFYSPOT+2*SMIXFSPACE,
		sw[3].name, sw[3].status);
	smix2rect = g_drwbtn(SMIXFXSPOT,SMIXFYSPOT+3*SMIXFSPACE,
		sw[6].name, sw[6].status);

	for(a = 0; a < 2; a++)
	{
	g_puts(DCOFXSPOT, DCOFYSPOT -8 + (a * DCOFSPACE),
	pm[a+2].name);
	g_hfader(DCOFXSPOT, DCOFYSPOT+(a*DCOFSPACE) , pm[a+2].value);
	}
	g_msgbox(DCOFXSPOT-16, DCOFYSPOT-12, 23*8, 3*DCOFSPACE+8,
	"Digital Controlled Oscillator", "");

	dcof0rect = g_drwbtn(DCOFXSPOT, DCOFYSPOT+DCOFSPACE*2,sw[10].status,"");

	dcof1rect = g_drwbtn(DCOFXSPOT+BWIDTH10,DCOFYSPOT+DCOFSPACE*2,
		sw[5].name, sw[5].status);

	for(a = 0; a < 5; a++)
	{
	g_puts(VCFFXSPOT, VCFFYSPOT -8 + (a * VCFFSPACE),
	pm[a+4].name);
	g_hfader(VCFFXSPOT, VCFFYSPOT+(a*VCFFSPACE) , pm[a+4].value);
	}
	g_msgbox(VCFFXSPOT-16, VCFFYSPOT-12, 23*8, 6*VCFFSPACE+8,
	"Voltage Controlled Filter", "");
	vcffrect = g_drwbtn(VCFFXSPOT, VCFFYSPOT+5*VCFFSPACE,
		sw[1].name,sw[1].status);

	for(a = 0; a < 4; a++)
	{
	g_puts(ENVFXSPOT, ENVFYSPOT -8 + (a * ENVFSPACE),
	pm[a+10].name);
	g_hfader(ENVFXSPOT, ENVFYSPOT+(a*ENVFSPACE) , pm[a+10].value);
	}
	g_msgbox(ENVFXSPOT-16, ENVFYSPOT-12, 23*8, 5*ENVFSPACE+8,
	"ADSR Envelope", "");
	envfrect = g_drwbtn(ENVFXSPOT, ENVFYSPOT+4*ENVFSPACE,
		sw[9].name,sw[9].status);

	}

loadpatch()
{
	FILE *fptr;
	char filename[80];
	int a;
	char temp[10];
	g_fselect("*.pat",filename);
	if( (fptr=fopen(filename, "r")) == NULL)
	{
	g_puts(0, 42*8,"Can't open file			");
	mshidecur();
	g_clrscn();
	menu();
	msshowcur();
	parameter();
	}
	for(a = 0; a<4; a++)
	{
	pm[a].value = atoi(fgets(temp,10,fptr));
/*	printf("%d\n", pm[a].value);*/
	}

	switchbyte[0] = atoi(fgets(temp,80,fptr));
/*	printf("%d\n", switchbyte[0]);	*/
	for(a = 4; a<15; a++)
	{
	pm[a].value = atoi(fgets(temp,80,fptr));
	/*  printf("%d\n", pm[a].value);	*/
	}

	switchbyte[1] = atoi(fgets(temp,80,fptr));
/*	printf("%x\n", switchbyte[1]);*/
	switchbyte[2] = atoi(fgets(temp,80,fptr));
/*	printf("%x\n", switchbyte[2]);	*/
	fgets(description, 80, fptr);
/*	printf("%s\n", description);*/
/*	getche();*/
	fclose(fptr);
	/* now to reassimilate the switch data */

if((switchbyte[0] & 0x40) == 0x40)  /* VCF */
	{
	sw[0].value = 1;
	sw[0].mask  = 0x40;
	strcpy(sw[0].status, "on ");
	}
else
	{
	sw[0].value = 0;
	sw[0].mask  = 0x00;
	strcpy(sw[0].status, "off");
	}

if((switchbyte[0] & 0x20) == 0x20)  /* VCA */
	{
	sw[1].value = 1;
	sw[1].mask  = 0x20;
	strcpy(sw[1].status, "on ");
	}
else
	{
	sw[1].value = 0;
	sw[1].mask  = 0x00;
	strcpy(sw[1].status, "off");
	}

if((switchbyte[1] & 0x20) == 0x20)  /* Chorus */
	{
	sw[2].value = 1;
	sw[2].mask  = 0x20;
	strcpy(sw[2].status, "off");
	}
else
	{
	sw[2].value = 0;
	sw[2].mask  = 0x00;
	strcpy(sw[2].status, "on ");
	}

if((switchbyte[1] & 0x10) == 0x10)  /* Sawtooth */
	{
	sw[3].value = 1;
	sw[3].mask  = 0x10;
	strcpy(sw[3].status, "on ");
	}
else
	{
	sw[3].value = 0;
	sw[3].mask  = 0x00;
	strcpy(sw[3].status, "off");
	}

if((switchbyte[1] & 0x08) == 0x08)  /* Pulse	*/
	{
	sw[4].value = 1;
	sw[4].mask  = 0x08;
	strcpy(sw[4].status, "on ");
	}
else
	{
	sw[4].value = 0;
	sw[4].mask  = 0x00;
	strcpy(sw[4].status, "off");
	}
/*printf(" %x, %x, %x", switchbyte[1], 0x04, switchbyte[1] & 0x04);*/
if((switchbyte[1] & 0x04) == 0x04)  /* Range 4 */
	{
/* printf("hello", getche());*/
	sw[5].value = 4;
	sw[5].mask  = 0x04;
	strcpy(sw[5].status, "4' ");
	}
else if((switchbyte[1] & 0x02) == 0x02)  /* Range 8 */
	{
	sw[5].value = 8;
	sw[5].mask  = 0x02;
	strcpy(sw[5].status, "8' ");
	}
else if((switchbyte[1] & 0x01) == 0x01)  /* Range 16 */
	{
	sw[5].value = 16;
	sw[5].mask  = 0x01;
	strcpy(sw[5].status, "16'");
	}
/*printf("%d, %x, %s \n",sw[5].value, sw[5].mask, sw[5].status);
getche();*/

if((switchbyte[2] & 0x20) == 0x20)  /* Noise	*/
	{
	sw[6].value = 1;
	sw[6].mask  = 0x20;
	strcpy(sw[6].status, "on ");
	}
else
	{
	sw[6].value = 0;
	sw[6].mask  = 0x00;
	strcpy(sw[6].status, "off");
	}

if((switchbyte[2] & 0x10) == 0x10)  /* HP filter*/
	{
	sw[7].value = 1;
	sw[7].mask  = 0x10;
	strcpy(sw[7].status, "off");
	}
else
	{
	sw[7].value = 0;
	sw[7].mask  = 0x00;
	strcpy(sw[7].status, "on ");
	}

if((switchbyte[2] & 0x04) == 0x04)  /* VCA source */
	{
	sw[8].value = 1;
	sw[8].mask  = 0x04;
	strcpy(sw[8].status, "GATE");
	}
else
	{
	sw[8].value = 0;
	sw[8].mask  = 0x00;
	strcpy(sw[8].status, "ENV ");
	}

if((switchbyte[2] & 0x02) == 0x02)  /* ENV dir */
	{
	sw[9].value = 1;
	sw[9].mask  = 0x02;
	strcpy(sw[9].status, "MINUS");
	}
else
	{
	sw[9].value = 0;
	sw[9].mask  = 0x00;
	strcpy(sw[9].status, "PLUS ");
	}

if((switchbyte[2] & 0x01) == 0x01)  /* PWM source */
	{
	sw[10].value = 1;
	sw[10].mask  = 0x01;
	strcpy(sw[10].status, "MAN");
	}
else
	{
	sw[10].value = 0;
	sw[10].mask  = 0x00;
	strcpy(sw[10].status, "LFO");
	}
}

sendpatch()
{
	int a;
	for(a = 0; a<3; a++)
	switchbyte[a] = 0;

	switchbyte[0] = (sw[0].mask | sw[1].mask);

	for(a = 2; a<6; a++)
	switchbyte[1] = (switchbyte[1] | sw[a].mask);

	for(a = 6; a<11; a++)
	switchbyte[2] = (switchbyte[2] | sw[a].mask);

/* for(a = 0; a<3; a++)
	printf("%d ", switchbyte[a]);
	getche();*/

	MIDIINIT
	txmidi( 0xF0);
	txmidi( 0x41);
	txmidi( 0x30);
	txmidi( channel);
	txmidi( patch);

	for(a = 0; a<4; a++)
	txmidi( pm[a].value);

	txmidi( switchbyte[0]);

	for(a = 4; a<15; a++)
	txmidi(pm[a].value);

	txmidi(switchbyte[1]);
	txmidi(switchbyte[2]);
	txmidi(0xF7);
	MIDIRESET
}
savepatch()
{
	FILE *fptr;
	char filename[80];
	int a;

	g_puts(0,42*8, "Enter name of patch to save > ");
	g_gets(-1,-1,filename);
	if( (fptr=fopen(filename, "w")) == NULL)
	{
	g_puts(0, 42*8,"Can't open file			");
	msshowcur();
	parameter();
	}
	g_puts(0,42*8,"Enter a description of the patch >");
	g_gets(-1,-1,description);
	g_puts(0,42*8,description);

	for(a = 0; a<4; a++)
	fprintf(fptr, "%d\n", pm[a].value);

	fprintf(fptr, "%d\n", switchbyte[0]);

	for(a = 4; a<15; a++)
	fprintf(fptr, "%d\n", pm[a].value);

	fprintf(fptr, "%d\n", switchbyte[1]);
	fprintf(fptr, "%d\n", switchbyte[2]);
	fprintf(fptr, "%s\n", description);

	fclose(fptr);
}
select_instr()
{
	if(instrument == 0) /*cycle through values*/
	{
	patch = 98;
	instrument = 1;
	}
	else if (instrument == 1)
	{
	patch = 17;
	instrument = 2;
	}
	else if (instrument == 2)
	{
	patch = 99;
	instrument = 0;
	}
	channel = sw[13+instrument].value - 1;
}
whole_mode()
{
	MIDIINIT
	if(sw[11].value == 1)
	{
	sw[11].value = 0;
	strcpy(sw[11].status, "off");
	txmidi(176 + channel);
	txmidi(121);
	txmidi(0);
	}
	else if(sw[11].value == 0)
	{
	sw[11].value = 1;
	strcpy(sw[11].status, "on ");
	txmidi(176 + channel);
	txmidi(121);
	txmidi(127);
	}
	MIDIRESET
}
hold()
{
	MIDIINIT
	if(sw[12].value == 1)
	{
	sw[12].value = 0;
	strcpy(sw[12].status, "off");
	txmidi(176 + channel);
	txmidi(64);
	txmidi(0);
	}
	else if(sw[12].value == 0)
	{
	sw[12].value = 1;
	strcpy(sw[12].status, "on ");
	txmidi(176 + channel);
	txmidi(64);
	txmidi(127);
	}
	MIDIRESET
}
set_channel()
{
char temp[80];
sw[13+instrument].value++;
if(sw[13+instrument].value > 16)
	sw[13+instrument].value = 1;

if (sw[13+instrument].value <10)
	{
	strcpy(sw[13+instrument].status, " ");
	itoa(sw[13+instrument].value, temp, 10);
	strcat(sw[13+instrument].status,temp);
	}
	else
	itoa(sw[13+instrument].value, sw[13+instrument].status, 10);

}



