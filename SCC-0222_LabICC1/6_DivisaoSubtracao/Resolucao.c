#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int a,b,div=0;
	scanf("%d",&a);
	scanf("%d",&b);
	while(a>=b){
		div++;
		a-=b;
	}
	printf("%d\n",div);
	printf("%d",a);
	return EXIT_SUCCESS;
}
