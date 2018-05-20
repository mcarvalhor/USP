#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

#define ADDITION_NUMBER 5

int main(){
	int i,R=0,N=0,S=1;
	char c,*Str=(char *)malloc(sizeof(char)*S);
	do{
		c=getchar();
		Str[N++]=c;
		if(N>=S){
			S+=ADDITION_NUMBER;
			Str=(char *)realloc(Str,sizeof(char)*S);
		}
	} while(c>=48 && c<=57);
	Str[N-1]='\0';
	for(i=0;i<strlen(Str);i++){
		R+=Str[i]-48;
	}
	printf("%d",R);
	free(Str);
	return EXIT_SUCCESS;
}
