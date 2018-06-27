#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N,i;
	double s=0;
	scanf(" %d",&N);
	for(i=N;i>0;i--){
		s+=i/(double)(N-i+1);
	}
	printf("%.4f",s);
	return EXIT_SUCCESS;
}
