#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N,i,P=1,M=0;
	float s=0;
	scanf("%d",&N);
	for(i=1;i<=N;i++){
		s+=P/(float)(N-M);
		P+=2;
		M++;
	}
	printf("%.4f",s);
	return EXIT_SUCCESS;
}
