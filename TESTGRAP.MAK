#TESTGRAP files

TESTGRAP.OBJ:	TESTGRAP.C
	cl /c /G2 /AL TESTGRAP.C

TESTGRAP.EXE:	TESTGRAP.OBJ MYGRAPH.LIB
	LINK TESTGRAP,TESTGRAP,NUL,MYPROC+MYGRAPH;

