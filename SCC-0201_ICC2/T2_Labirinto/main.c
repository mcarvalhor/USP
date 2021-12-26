#include <stdlib.h>
#include <stdio.h>
#include "labyrinth.h"



/*
 *	~ Labirinto com Tesouro ~
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
	LABYRINTH *Lab;
	Lab=ReadLab(stdin); // Ler labirinto da entrada padrão.
	FindSolutionsWithoutTreasure(Lab); // Achar soluções sem passar pela câmara de tesouro.
	FindSolutionsWithTreasure(Lab); // Achar soluções passando pela câmara de tesouro.
	SortSolutions(Lab); // Ordenar as soluções encontradas.
	PrintSolutions(Lab,stdout); // Imprimir as soluções na saída de texto.
	DestroyLab(Lab); // Limpar memória usada.
	return EXIT_SUCCESS;
}



