#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

#define N_DIAS_NO_MES 31
int main(){
	char Word[300],is_palindrome=1;
	int i,Word_len;
	fgets(Word,sizeof(Word)-1,stdin);
	Word_len=strlen(Word);
	Word[Word_len-1]='\0';
	Word_len-=1;
	for(i=0;i<Word_len;i++){
		if(!(Word[i]==Word[Word_len-(i+1)])){
			is_palindrome=0;
			break;
		}
	}
	if(is_palindrome){
		printf("SIM");
	}else{
		printf("NAO");
	}
	return EXIT_SUCCESS;
}