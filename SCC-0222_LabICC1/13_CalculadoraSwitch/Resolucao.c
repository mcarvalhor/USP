#include <stdio.h>
#include <stdlib.h>

/*
Matheus
mcarvalhor@usp.br
*/

int main(){
	float a,b;
	char o;
	scanf("%f %s %f",&a,&o,&b);
	switch(o){
		case '+':
			printf("%.2f",a+b);
			break;
		case '-':
			printf("%.2f",a-b);
			break;
		case '*':
			printf("%.2f",a*b);
			break;
		case '/':
			printf("%.2f",a/b);
			break;
	}
	return EXIT_SUCCESS;
}
