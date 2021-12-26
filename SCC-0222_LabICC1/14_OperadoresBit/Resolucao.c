#include <stdio.h>
#include <stdlib.h>

/*
Matheus
mcarvalhor@usp.br
*/

int main(){
	int a,b;
	scanf("%d %d",&a,&b);
	printf("%d\n",a&b);
	printf("%d\n",a|b);
	printf("%d\n",a^b);
	printf("%d\n",~a);
	printf("%d\n",~b);
	printf("%d\n",a>>2);
	printf("%d",b<<2);
	return EXIT_SUCCESS;
}
