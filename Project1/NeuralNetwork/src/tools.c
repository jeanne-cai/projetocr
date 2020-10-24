#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cleanBuffer()
{
  int c;
  while((c=getchar())!='\n' && c!= EOF);
}
void secuScanf(char* format, void* ref){
	scanf(format,ref);
	cleanBuffer();
}