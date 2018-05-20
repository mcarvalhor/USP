#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	unsigned int Nc=0,i;
	int Ni[10000],Nr; // REVER O TAMANHO DA ARRAY!
	double Mean=0,T=0;
	while(1){
		scanf("%d",&Nr);
		if(Nr){
			Ni[Nc]=Nr;
			Mean+=Nr;
			Nc++;
		}else{
			break;
		}
	}
	Mean=Mean/(double)Nc;
	for(i=0;i<Nc-2;i++){
		T=pow(Ni[i+1],2)/(double)(Mean*((Ni[i]+Ni[i+1]+Ni[i+2])/3.0));
		printf("%.4lf\n",T);
	}
	return EXIT_SUCCESS;
}