#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

#define ADDITION_NUMBER 10

int main(){
	int i,N=0,S=1;
	char c,*Str=(char *)malloc(sizeof(char)*S);
	do{
		c=getchar();
		Str[N++]=c;
		if(N>=S){
			S+=ADDITION_NUMBER;
			Str=(char *)realloc(Str,sizeof(char)*S);
		}
	} while(c!='\n');
	Str[N-1]='\0';
	for(i=0;i<strlen(Str);i++){
		if(i){
			printf("\n%c",Str[i]);
		}else{
			printf("%c",Str[i]);
		}
	}
	free(Str);
	return EXIT_SUCCESS;
}
