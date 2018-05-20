#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N;
	float s=0;
	scanf("%d",&N);
	for(N=N;N>0;N--){
		if(N%2){
			s+=1/(float)N;
		}else{
			s-=1/(float)N;
		}
	}
	printf("%.4f",s);
	return EXIT_SUCCESS;
}
