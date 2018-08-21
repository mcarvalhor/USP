#include <stdlib.h>
#include <stdio.h>
#include <matrioska.h>



/*
 *	~       MATRIOSKA       ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



int main(int argc, char **argv){
	int i,N,MatrioskaN,R;
	scanf("%d",&N);
	for(i=0;i<N;i++){ // Para cada possível sequência Matrioska...
		if(i)	printf("\n"); // Imprimir o pulo de linha de forma adequada.
		if( (MatrioskaN=Matrioska_Check(stdin))>=0 ){ // A sequência Matrioska é válida?
			printf("=) Matrioska %d",MatrioskaN); // Sim, imprimir o número de Matrioska/s possível/is.
		}else{
			printf("=( Tente novamente"); // Não.
			if(MatrioskaN==-2)
				do { scanf("%d",&R); } while(R!=0); // Se não tiver chegado no fim da sequência, chegar.
		}
	}
	return EXIT_SUCCESS;
}



