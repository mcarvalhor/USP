#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,j,N,Rl=0,Rc,Rd1,Rd2;
	scanf("%d",&N);
	int Mat[N][N];
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			scanf("%d",&Mat[i][j]);
		}
	}
	Rd1=0;
	Rd2=0;
	for(i=0;i<N;i++){
		Rd1+=Mat[i][i];
		Rd2+=Mat[N-(i+1)][N-(i+1)];
	}
	if(Rd1!=Rd2){
		printf("NAO\n");
		return EXIT_SUCCESS;
	}
	for(i=0;i<N;i++){
		Rl=0;
		Rc=0;
		for(j=0;j<N;j++){
			Rl+=Mat[i][j];
			Rc+=Mat[j][i];
		}
		if((Rl!=Rc) || (Rl!=Rd1)){
			printf("NAO\n");
			return EXIT_SUCCESS;
		}
	}
	printf("SIM\n");
	return EXIT_SUCCESS;
}
