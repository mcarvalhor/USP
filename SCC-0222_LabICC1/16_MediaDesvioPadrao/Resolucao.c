#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N,i;
	scanf(" %d",&N);
	double Ni[N],M=0,DP=0;
	for(i=0;i<N;i++){
		scanf(" %lf",&Ni[i]);
		M+=Ni[i];
	}
	M=M/(double)N;
	for(i=0;i<N;i++){
		DP+=pow((Ni[i]-M),2);
	}
	DP=sqrt(DP/(double)N);
	printf("%.4lf\n",M);
	printf("%.4lf",DP);
	return EXIT_SUCCESS;
}
