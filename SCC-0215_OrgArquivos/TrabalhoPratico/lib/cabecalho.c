#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "cabecalho.h"



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



void lerCabecalho(FILE *arquivoDAT) {
	/*
	* Esta função lê para 'cab' o cabeçalho contido no arquivo 'arquivoDAT'.
	*/
	char aux[TAMANHO_CABECALHO];

	fseek(arquivoDAT, 0, SEEK_SET);
	fread(aux, TAMANHO_CABECALHO, 1, arquivoDAT);

	memcpy(&cab.status, aux, sizeof(char));
	memcpy(&cab.topoPilha, aux + sizeof(char), sizeof(int));
}

void escreverCabecalho(FILE *arquivoDAT) {
	/*
	* Esta função escreve o cabeçalho 'cab' no arquivo 'arquivoDAT'.
	*/
	char aux[TAMANHO_CABECALHO];

	fseek(arquivoDAT, 0, SEEK_SET);
	memcpy(aux, &cab.status, sizeof(char));
	memcpy(aux + sizeof(char), &cab.topoPilha, sizeof(int));

	fwrite(aux, TAMANHO_CABECALHO, 1, arquivoDAT);
}


void lerCabecalhoIndice(FILE *arquivoIndice) {
	/*
	* Esta função lê para 'cabIndice' o cabeçalho de índice contido no arquivo 'arquivoIndice'.
	*/
	char aux[TAMANHO_CABECALHO_INDICE];

	fseek(arquivoIndice, 0, SEEK_SET);
	fread(aux, TAMANHO_CABECALHO_INDICE, 1, arquivoIndice);

	memcpy(&cabIndice.status, aux, sizeof(char));
	memcpy(&cabIndice.noRaiz, aux + sizeof(char), sizeof(int));
	memcpy(&cabIndice.altura, aux + sizeof(char) + sizeof(int), sizeof(int));
	memcpy(&cabIndice.ultimoRRN, aux + sizeof(char) + sizeof(int) * 2, sizeof(int));
}

void escreverCabecalhoIndice(FILE *arquivoIndice) {
	/*
	* Esta função escreve o cabeçalho de índice 'cabIndice' no arquivo 'arquivoIndice'.
	*/
	char aux[TAMANHO_CABECALHO_INDICE];

	fseek(arquivoIndice, 0, SEEK_SET);
	memcpy(aux, &cabIndice.status, sizeof(char));
	memcpy(aux + sizeof(char), &cabIndice.noRaiz, sizeof(int));
	memcpy(aux + sizeof(char) + sizeof(int), &cabIndice.altura, sizeof(int));
	memcpy(aux + sizeof(char) + sizeof(int) * 2, &cabIndice.ultimoRRN, sizeof(int));

	fwrite(aux, TAMANHO_CABECALHO_INDICE, 1, arquivoIndice);
}


