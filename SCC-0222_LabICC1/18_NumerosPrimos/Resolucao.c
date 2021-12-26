#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	unsigned int i,j,n;
	char is_prime;
	scanf("%d",&n);
	for(i=2;i<=n;i++){
		is_prime=1;
		for(j=2;j<=sqrt(n);j++){
			if(i!=j && !(i%j)){
				is_prime=0;
				break;
			}
		}
		if(is_prime){
			if(i!=2){
				printf(" %d",i);
			}else{
				printf("%d",i);
			}
		}
	}
	printf("\n");
	return EXIT_SUCCESS;
}