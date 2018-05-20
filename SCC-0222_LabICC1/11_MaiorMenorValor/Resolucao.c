#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N,Ni,Max,Min;
	scanf("%d",&N);
	scanf("%d",&Ni);
	Max=Ni;
	Min=Ni;
	N--;
	for(N=N;N>0;N--){
		scanf("%d",&Ni);
		if(Ni>Max){
			Max=Ni;
		}
		if(Ni<Min){
			Min=Ni;
		}
	}
	printf("max: %d\n",Max);
	printf("min: %d",Min);
	return EXIT_SUCCESS;
}
