
#ifndef REGISTER_H_
#define REGISTER_H_
#include <stdio.h>

	typedef struct {
		int RRN;
		char removido;
		char estadoOrigem[85];
		char estadoDestino[85];
		int distancia;
		char cidadeOrigem[85];
		char cidadeDestino[85];
		char tempoViagem[85];
		/*char origem[85];
		char destino[85];*/
	} REGISTRO_T;

	typedef struct {
		char status;
		int numeroVertices;
		int numeroArestas;
		char dataUltimaCompactacao[11];
	} CABECALHO_T;

	int R_lerCabecalho(CABECALHO_T *dest, FILE *stream);
	int R_escreverCabecalho(CABECALHO_T *src, FILE *stream); // Se não existe ainda, escreve. Se já existe, reescreve.

	int R_lerRegistro(REGISTRO_T *dest, FILE *stream);
	int R_escreverRegistro(REGISTRO_T *src, FILE *stream);
	int R_reescreverRegistro(REGISTRO_T *src, FILE *stream);

#endif