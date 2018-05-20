#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	short int i,R;
	for(i=1000;i<9999;i++){
		R=(i/100)+(i-((i/100)*100));
		if(sqrt(i)==R){
			printf("%hi\n",i);
		}
	}
	return EXIT_SUCCESS;
}
