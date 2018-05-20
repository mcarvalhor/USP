#include <stdio.h>
#include <stdlib.h>

/*
Por Matheus Carvalho Raimundo.
mcarvalhor@usp.br
*/

int main(){
	int a,b,c,maior;
	scanf("%d",&a);
	maior=a;
	scanf("%d",&b);
	if(b>maior){
		 maior=b;
	}
	scanf("%d",&c);
	if(c>maior){
		maior=c;
	}
	printf("%d",maior);
	return EXIT_SUCCESS;
}
