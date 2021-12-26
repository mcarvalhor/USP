#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Por Matheus Carvalho Raimundo.
mcarvalhor@usp.br
*/

int main(){
	unsigned int N,i,k;
	scanf("%d",&N);
	float Ni[N];
	double DCT;
	for(k=0;k<N;k++){
		scanf("%f",&Ni[k]);
	}
	for(k=0;k<N;k++){
		DCT=0;
		for(i=0;i<N;i++){
			DCT+=Ni[i]*cos((M_PI/N)*(i+0.5)*k);
		}
		printf("%.6lf\n",DCT);
	}
	return EXIT_SUCCESS;
}
