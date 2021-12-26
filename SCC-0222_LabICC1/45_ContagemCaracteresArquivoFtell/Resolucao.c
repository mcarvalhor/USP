#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N;
	FILE *Stream;
	char FileName[100];
	scanf("%s",FileName);
	FileName[99]='\0';
	Stream=fopen(FileName,"r");
	fseek(Stream,0,SEEK_END);
	N=ftell(Stream);
	fclose(Stream);
	printf("%d",N);
	return EXIT_SUCCESS;
}
