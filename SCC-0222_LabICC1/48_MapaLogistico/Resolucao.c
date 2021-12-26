#include <stdlib.h>
#include <stdio.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,K;
	double X0,r;
	scanf("%lf %lf %d",&X0,&r,&K);
	for(i=1;i<=K;i++){
		X0=r*X0*(1-X0);
		printf("%d %.6lf\n",i,X0);
	}
	return EXIT_SUCCESS;
}
