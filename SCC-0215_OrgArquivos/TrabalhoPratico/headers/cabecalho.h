
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

#ifndef CABECALHO_H_
#define CABECALHO_H_
#include <stdio.h>


	#define TAMANHO_CABECALHO 5
	#define TAMANHO_CABECALHO_INDICE 13


	typedef struct {
		char status;
		int topoPilha;
	} CAB;

	typedef struct {
		char status;
		int noRaiz;
		int altura;
		int ultimoRRN;
	} CAB_INDICE;


	CAB cab; /* Cabeçalho do arquivo de dados */
	CAB_INDICE cabIndice; /* Cabeçalho do arquivo de índice da árvore-B */


	void lerCabecalho(FILE *);

	void escreverCabecalho(FILE *);

	void lerCabecalhoIndice(FILE *);

	void escreverCabecalhoIndice(FILE *);


#endif
