#include <stdio.h>
#include <stdlib.h>
#include "soccer.h"



/*
 *	~        FUTEBOL        ~
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
	SOCCER_TOUR *Aux;
	scanf("%d",&N); // Quantos torneios serão?
	for(i=0;i<N;i++){ // Para cada torneio...
		if(i>0)	printf("\n");
		Aux=S_NewFrom(stdin); // Ler torneio da entrada padrão.
		S_Calc(Aux); // Calcular pontuação de cada equipe.
		S_Sort(Aux); // Ordenar da forma correta.
		S_Print(Aux,stdout); // Imprimir na saída padrão.
		S_Destroy(Aux); // Limpar da memória.
	}
	return EXIT_SUCCESS;
}



