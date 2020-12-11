#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanBuffer()
{
  int c;
  while((c=getchar())!='\n' && c!= EOF);
}
void secuScanf(char* format, void* ref){
	int retour;
	retour=scanf(format,ref);
	if(retour==0)
	{
		printf("secuScanf:ereur\n");
		exit(-1);
	}
	cleanBuffer();
}