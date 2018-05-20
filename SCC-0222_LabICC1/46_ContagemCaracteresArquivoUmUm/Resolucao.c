#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N=0;
	FILE *Stream;
	char FileName[100];
	scanf("%s",FileName);
	FileName[99]='\0';
	Stream=fopen(FileName,"r");
	while(!feof(Stream)){
		getc(Stream);
		N++;
	}
	fclose(Stream);
	printf("%d",--N);
	return EXIT_SUCCESS;
}
