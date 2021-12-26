#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N,i,j;
	scanf("%d",&N);
	//getchar();
	char Str[N][1000],Rc[1000];	// Str[2][3] == *(Str+2*N+3)
	for(i=0;i<N;i++){
		scanf("%s",Str[i]);
	}
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			if(strcmp(Str[i],Str[j])>0){
				strcpy(Rc,Str[i]);
				strcpy(Str[i],Str[j]);
				strcpy(Str[j],Rc);
			}
		}
		printf("%s\n",Str[i]);
	}
	return EXIT_SUCCESS;
}
