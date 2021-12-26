#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int Contar(char *str,char chpatt){
	int i,count=0;
	for(i=0;i<strlen(str);i++){
		if(str[i]==chpatt){
			count++;
		}
	}
	return count;
}

int main(){
	char Str[1000],ChPatt;
	fgets(Str,sizeof(Str),stdin);
	scanf(" %c",&ChPatt);
	printf("%d",Contar(Str,ChPatt));
	return EXIT_SUCCESS;
}
