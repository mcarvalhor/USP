#include <stdio.h>
#include <stdlib.h>

/*
Por Matheus Carvalho Raimundo.
mcarvalhor@usp.br
*/

int main(){
	unsigned int N,i;
	scanf("%d",&N);
	float Ni;
	double MHA=0;
	for(i=0;i<N;i++){
		scanf("%f",&Ni);
		MHA+=1.0/(double)(Ni+1);
	}
	MHA=(N/(double)(MHA))-1;
	printf("%.2f",MHA);
	return EXIT_SUCCESS;
}
