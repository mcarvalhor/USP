#include <stdlib.h>
#include <stdio.h>
#include <cards.h>



/*
 *	~         CARTAS        ~
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
	int i,N;
	LIST *Aux;
	scanf("%d",&N);
	for(i=0;i<N;i++){ // Para cada possível sequência de cartas...
		Aux=ReadCardsFrom(stdin); // Ler a sequência.
		ProcessCards(stdout,Aux); // Processar e imprimir.
		DestroyCards(Aux); // Remover da memória.
		printf("\n");
	}
	return EXIT_SUCCESS;
}



