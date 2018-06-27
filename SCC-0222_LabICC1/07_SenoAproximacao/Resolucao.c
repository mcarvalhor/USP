#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

unsigned long math_factorial(unsigned char n){
	unsigned long factorial;
	for(factorial=1;n>1;n--){
		factorial=factorial*n;
	}
	return factorial;
}

int main(){
	int i=32,div=3;
	float a;
	double s;
	scanf(" %f",&a);
	s=a;
	for(i=i;i>0;i--){
		if(i%2){
			s+=pow(a,div)/(double)math_factorial(div);
		}else{
			s-=pow(a,div)/(double)math_factorial(div);
		}
		div+=2;
	}
	printf("%.6f",(float)s);
	return EXIT_SUCCESS;
}
