
/*
 *	~       Trabalho Prático: Parte 2       ~
 *
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/

#ifndef ARVOREB_H_
#define ARVOREB_H_
#include <stdio.h>


	void iniciarBufferPool();

	void finalizarBufferPool(char *, FILE *);

	int buscaRRN(int, FILE *);

	void inserirChave(int, int, FILE *);

	int removerChave(int, FILE *);


#endif
