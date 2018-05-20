#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,N;
	float O[2],R[2],distancia=0;
	scanf("%d",&N);
	for(i=0;i<N;i++){
		if(i){
			O[0]=R[0];
			O[1]=R[1];
		}
		scanf("%f %f",&R[0],&R[1]);
		if(i){
			distancia+=sqrt(pow(R[0]-O[0],2)+pow(R[1]-O[1],2));
		}
	}
	printf("%.4f",distancia);
	return EXIT_SUCCESS;
}