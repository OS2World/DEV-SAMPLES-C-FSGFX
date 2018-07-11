/* mymisc.c */
/* miscellaneous functions which are missing or don't function
properly from MSC 5.1 */
extern long myfilelength(int handle);
long myfilelength(int handle)
{
	long oldposition,newposition;
	oldposition = lseek(handle,0L,SEEK_CUR); save current position 
	newposition = lseek(handle,0L,SEEK_END); determine length of file 
	lseek(handle,oldposition,SEEK_SET);      restore current position 
	return(newposition);
}
int strindex(s, t)
char s[];
char t[];
{
int i, j, k;

for (i = 0; s[i] != '\0'; i++)
{
for (j = i, k = 0; t[k] != '\0' && s[j]==t[k]; j++, k++)
	;
	if (k>0 && t[k] == '\0')
		return i;
}
return -1;
}


convertuchartobin(a,temp)
unsigned char a;
char temp[];
{
        int i;
        unsigned char b;
        b = 0x80;
        for(i = 0; i<8; i++)
                {
                if(a & b)
                        temp[i] = '1';
                else
                        temp[i] = '0';
                b = b >> 1;
                }
}

int rjustifystring(char string[])
{
	int i,j;
	char temp[80];
	j = strlen(string);
	if (j < 8)
	{
		for(i=0; i <(8-j); i++)
			temp[i] = 0x20;
		temp[i] = 0;
		strcat(temp,string);
		strcpy(string,temp);
	}
}

