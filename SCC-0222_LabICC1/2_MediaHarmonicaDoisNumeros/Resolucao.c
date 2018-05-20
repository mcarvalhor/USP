#include <stdio.h>
#include <stdlib.h>

/*
Por Matheus Carvalho Raimundo.
mcarvalhor@usp.br
*/

int main(){
	float a,b,mh;
	scanf("%f",&a);
	scanf("%f",&b);
	mh=(2/( (1/(a+1))+(1/(b+1)) ))-1;
	printf("%.2f",mh);
	return EXIT_SUCCESS;
}
