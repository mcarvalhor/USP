#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* 
Matheus
mcarvalhor@usp.br
*/

int main(){
	int a,b,c,calc;
	float root1,root2;
	scanf("%d %d %d",&a,&b,&c);
	calc=(pow(b,2)-4*a*c);
	if(calc>0){
		root1=((-b)+sqrt(calc))/(2.0*a);
		root2=((-b)-sqrt(calc))/(2.0*a);
		if(root1>root2)
			printf("%.3f %.3f",root2,root1);
		else
			printf("%.3f %.3f",root1,root2);
	}else if(calc<0){
		printf("NAO EXISTE RAIZ REAL");
	}else{
		root1=(-b)/(2.0*a);
		printf("%.3f",root1);
	}
	return EXIT_SUCCESS;
}
