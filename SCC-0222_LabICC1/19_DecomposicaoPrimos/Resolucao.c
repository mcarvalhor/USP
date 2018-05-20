#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	unsigned int i,j,N,n,nc;
	char is_prime;
	scanf("%d",&N);
	n=N;
	for(i=2;i<=N;i++){
		nc=0;
		is_prime=1;
		for(j=2;j<=sqrt(N);j++){
			if(i!=j && !(i%j)){
				is_prime=0;
				break;
			}
		}
		if(is_prime){
			while(!(n%i)){
				n=n/i;
				nc++;
			}
			if(nc>0){
				printf("%d %d \n",i,nc);
			}
		}
	}
	return EXIT_SUCCESS;
}