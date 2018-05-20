#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

#define ADDITION_NUMBER 5

#define bool char
#define true 1
#define false 0

bool DentroDe(char *Pattern,char *Str){
	int i,j;
	for(i=0;i<strlen(Str);i++){
		for(j=0;j<strlen(Pattern);j++){
			if(Str[i+j]!=Pattern[j]){
				break;
			}
		}
		if(j==strlen(Pattern)){
			return true;
		}
	}
	return false;
}

int main(){
	int N,S=1;
	char c,*N1=(char *)malloc(sizeof(char)*S),*N2=(char *)malloc(sizeof(char)*S);
	N=0;
	do{
		c=getchar();
		N1[N++]=c;
		if(N>=S){
			S+=ADDITION_NUMBER;
			N1=(char *)realloc(N1,sizeof(char)*S);
		}
	} while(c>=48 && c<=57);
	N1[N-1]='\0';
	do{
		c=getchar();
	} while(!(c>=48 && c<=57));
	N2[0]=c;
	N=1;
	S=1;
	do{
		c=getchar();
		N2[N++]=c;
		if(N>=S){
			S+=ADDITION_NUMBER;
			N2=(char *)realloc(N2,sizeof(char)*S);
		}
	} while(c>=48 && c<=57);
	N2[N-1]='\0';
	if(atoi(N1)>=atoi(N2)){
		printf("%s %s ",N2,N1);
		if(DentroDe(N2,N1)==true){
			printf("SIM");
		}else{
			printf("NAO");
		}
	}else{
		printf("%s %s ",N1,N2);
		if(DentroDe(N1,N2)==true){
			printf("SIM");
		}else{
			printf("NAO");
		}
	}
	free(N1);
	free(N2);
	return EXIT_SUCCESS;
}
