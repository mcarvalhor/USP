#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,letter_count=0,int_count=0;
	char str[2000];
	fgets(str,sizeof(str),stdin);
	for(i=0;i<strlen(str);i++){
		if( ((str[i]>=65) && (str[i]<=90)) || ((str[i]>=97) && (str[i]<=122)) ){
			letter_count++;
		}else if((str[i]>=48) && (str[i]<=57)){
			int_count++;
		}
	}
	printf("%d\n%d",letter_count,int_count);
	return EXIT_SUCCESS;
}