#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i;
	char str[2000];
	fgets(str,sizeof(str),stdin);
	for(i=0;i<strlen(str);i++){
		if((str[i]>=97) && (str[i]<=122)){
			str[i]=str[i]-32;
		}
	}
	printf("%s",str);
	return EXIT_SUCCESS;
}