#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fornecido.h"
#include "consts.h"
#include "register.h"
#include "register2.h"
#include "index.h"
#include "object-to-int.h"
#include "deque.h"


typedef struct __CAMPO_BUSCA {
	char nomeCampo[100];
	char valorCampo[100];
	struct __CAMPO_BUSCA *next;
} CAMPO_BUSCA;

typedef struct __GRAFO_ID {
	int idPessoa;
	char nomePessoa[100];
} GRAFO_ID;

int busca_match(REG_DATA *reg, CAMPO_BUSCA *campo) {
	while(campo != NULL) {
		if(reg->removed == R_REMOVIDO && strcmp(campo->nomeCampo, "RRN") != 0) {
			campo = campo->next;
			continue;
		}
		if(strcmp(campo->nomeCampo, "idPessoa") == 0) {
			if(atoi(campo->valorCampo) == reg->idPessoa) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "nomePessoa") == 0) {
			if(strcmp(campo->valorCampo, reg->nomePessoa) == 0) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "idadePessoa") == 0) {
			if(campo->valorCampo[0] == '\0' && reg->idadePessoa < 0) {
				return 1;
			} else if(campo->valorCampo[0] != '\0' && atoi(campo->valorCampo) == reg->idadePessoa) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "twitterPessoa") == 0) {
			if(strcmp(campo->valorCampo, reg->twitterPessoa) == 0) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "RRN") == 0) {
			if(atoi(campo->valorCampo) == reg->RRN) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "removido") == 0) {
			if(reg->removed == R_REMOVIDO) {
				return 1;
			}
		} else if(strcmp(campo->nomeCampo, "true") == 0) {
			return 1;
		}
		campo = campo->next;
	}
	return 0;
}

int busca_replace(REG_DATA *reg, CAMPO_BUSCA *campo) {
	while(campo != NULL) {
		if(reg->removed == R_REMOVIDO) {
			campo = campo->next;
			continue;
		}
		if(strcmp(campo->nomeCampo, "idPessoa") == 0) {
			reg->idPessoa = atoi(campo->valorCampo);
		} else if(strcmp(campo->nomeCampo, "nomePessoa") == 0) {
			strcpy(reg->nomePessoa, campo->valorCampo);
		} else if(strcmp(campo->nomeCampo, "idadePessoa") == 0) {
			if(campo->valorCampo[0] == '\0') {
				reg->idadePessoa = -1;
			} else {
				reg->idadePessoa = atoi(campo->valorCampo);
			}
		} else if(strcmp(campo->nomeCampo, "twitterPessoa") == 0) {
			strcpy(reg->twitterPessoa, campo->valorCampo);
		} else if(strcmp(campo->nomeCampo, "removido") == 0) {
			reg->removed = R_REMOVIDO;
		}
		campo = campo->next;
	}
}

int ordena_comparador(REG2_DATA *A, REG2_DATA *B) {
	if(A->idPessoaQueSegue != B->idPessoaQueSegue) {
		return A->idPessoaQueSegue - B->idPessoaQueSegue;
	}
	if(A->idPessoaQueESeguida != B->idPessoaQueESeguida) {
		return A->idPessoaQueESeguida - B->idPessoaQueESeguida;
	}
	if(strcmp(A->dataInicioQueSegue, B->dataInicioQueSegue) != 0) {
		return strcmp(A->dataInicioQueSegue, B->dataInicioQueSegue);
	}
	return strcmp(A->dataFimQueSegue, B->dataFimQueSegue);
}

int grafo_comparador(GRAFO_ID *A, GRAFO_ID *B) {
	return A->idPessoa - B->idPessoa;
}

void f1_readCsv(char *csvPath, char *dataPath, char *indexPath) {
	FILE *csvStream, *fStream;
	REG_HEADER header;
	REG_DATA reg;
	IND_T *index;
	char aux[50];

	if((csvStream = fopen(csvPath, "r")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	if((fStream = fopen(dataPath, "wb")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(csvStream);
		return;
	}

	if((index = I_init(NULL)) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(csvStream);
		fclose(fStream);
		return;
	}

	header.status = STATUS_INCONSISTENTE;
	header.quantidadePessoas = 0;
	R_writeH(&header, fStream);

	fscanf(csvStream, "%*[^\r\n]"); /* Pular primeira linha. */
	while(fscanf(csvStream, "%d%*c", &reg.idPessoa) == 1) {
		if(fscanf(csvStream, "%[^,]", reg.nomePessoa) != 1) {
			reg.nomePessoa[0] = '\0';
		}
		if(fscanf(csvStream, "%*c%[^,]", aux) == 1) {
			reg.idadePessoa = atoi(aux);
		} else {
			reg.idadePessoa = -1;
		}
		fscanf(csvStream, "%*c%s", reg.twitterPessoa);
		R_write(&reg, fStream);
		I_push(&reg, index);
		header.quantidadePessoas++;
	}

	header.status = STATUS_CONSISTENTE;
	R_writeH(&header, fStream);
	fclose(csvStream);
	fclose(fStream);

	if(I_write(index, indexPath) == 0) {
		printf("Falha no carregamento do arquivo.\n");
	} else {
		binarioNaTela1(dataPath, indexPath);
	}
	I_destroy(index);
}

void f2_listAll(char *dataPath) {
	REG_HEADER header;
	FILE *fStream;
	REG_DATA reg;
	char found;

	if((fStream = fopen(dataPath, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(R_readH(&header, fStream) == 0 || header.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	found = 0;
	while(R_read(&reg, fStream) == 1) {
		if(reg.removed == R_REMOVIDO) {
			continue;
		}
		R_print(&reg);
		found = 1;
	}

	if(found == 0) {
		printf("Registro inexistente.\n");
	}

	fclose(fStream);
}

void f3_search(char *dataPath, char *indexPath) {
	CAMPO_BUSCA criterio;
	REG_HEADER header;
	FILE *fStream;
	IND_DATA *RRN;
	REG_DATA reg;
	IND_T *index;
	char found;

	if((fStream = fopen(dataPath, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(R_readH(&header, fStream) == 0 || header.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	if((index = I_init(indexPath)) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	criterio.next = NULL;
	scanf("%s", criterio.nomeCampo);
	scan_quote_string(criterio.valorCampo);

	if(strcmp(criterio.nomeCampo, "idPessoa") == 0) { /* Caso especial: usar índice primário. */
		if((RRN = I_find(atoi(criterio.valorCampo), index)) == NULL) {
			printf("Registro inexistente.\n");
			I_destroy(index);
			fclose(fStream);
			return;
		}
		if(fseek(fStream, 64 + 64 * RRN->RRN, SEEK_SET) != 0 || R_read(&reg, fStream) != 1 || reg.removed == R_REMOVIDO) {
			printf("Registro inexistente.\n");
			I_destroy(index);
			fclose(fStream);
			return;
		}
		R_print(&reg);
		I_destroy(index);
		fclose(fStream);
		return;
	}
	I_destroy(index);

	found = 0;
	while(R_read(&reg, fStream) == 1) {
		if(reg.removed == R_REMOVIDO || busca_match(&reg, &criterio) == 0) {
			continue;
		}
		R_print(&reg);
		found = 1;
	}

	if(found == 0) {
		printf("Registro inexistente.\n");
	}

	fclose(fStream);
}

void f4_insert(char *dataPath, char *indexPath) {
	REG_HEADER header;
	FILE *fStream;
	REG_DATA reg;
	IND_T *index;
	char aux[50];
	int i, N;

	if((fStream = fopen(dataPath, "rb+")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(R_readH(&header, fStream) == 0 || header.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	if((index = I_init(indexPath)) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	/* Torna inconsistente */
	header.status = STATUS_INCONSISTENTE;
	R_writeH(&header, fStream);

	scanf("%d", &N);
	fseek(fStream, 0, SEEK_END);
	for(i = 0; i < N; i++) {
		scanf("%d", &reg.idPessoa);
		scan_quote_string(reg.nomePessoa);
		scan_quote_string(aux);
		if(aux[0] == '\0') {
			reg.idadePessoa = -1;
		} else {
			reg.idadePessoa = atoi(aux);
		}
		scan_quote_string(reg.twitterPessoa);
		R_write(&reg, fStream);
		I_push(&reg, index);
		header.quantidadePessoas++;
	}

	/* Torna consistente */
	header.status = STATUS_CONSISTENTE;
	R_writeH(&header, fStream);

	I_rewrite(index);
	I_destroy(index);
	fclose(fStream);
	binarioNaTela1(dataPath, indexPath);
}

void f5_update(char *dataPath, char *indexPath) {
	CAMPO_BUSCA *criterios, **substituicoes;
	REG_HEADER header;
	int i, j, N, M;
	FILE *fStream;
	REG_DATA reg;
	IND_T *index;
	char found;

	if((fStream = fopen(dataPath, "rb+")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(R_readH(&header, fStream) == 0 || header.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	if((index = I_init(indexPath)) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}
	I_destroy(index);

	if((index = I_init(NULL)) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	scanf("%d", &N);
	criterios = (CAMPO_BUSCA *) malloc(sizeof(CAMPO_BUSCA) * N);
	substituicoes = (CAMPO_BUSCA **) malloc(sizeof(CAMPO_BUSCA *) * N);
	for(i = 0; i < N; i++) {
		scanf("%s", criterios[i].nomeCampo);
		scan_quote_string(criterios[i].valorCampo);
		scanf("%d", &M);
		criterios[i].next = NULL;
		if(M < 1) {
			substituicoes[i] = NULL;
			continue;
		}
		substituicoes[i] = (CAMPO_BUSCA *) malloc(sizeof(CAMPO_BUSCA) * M);
		for(j = 0; j < M; j++) {
			scanf("%s", substituicoes[i][j].nomeCampo);
			scan_quote_string(substituicoes[i][j].valorCampo);
		}
		for(j = 0; j < M - 1; j++) {
			substituicoes[i][j].next = &substituicoes[i][j + 1];
		}
		substituicoes[i][M - 1].next = NULL;
	}

	/* Torna inconsistente */
	header.status = STATUS_INCONSISTENTE;
	R_writeH(&header, fStream);

	header.quantidadePessoas = 0;
	while(R_read(&reg, fStream) == 1) {
		if(reg.removed == R_REMOVIDO) {
			continue;
		}
		found = 0;
		for(i = 0; i < N; i++) {
			if(busca_match(&reg, &criterios[i]) == 1) {
				found = 1;
				busca_replace(&reg, substituicoes[i]);
			}
		}
		if(found == 1) {
			R_rewrite(&reg, fStream);
		}
		if(reg.removed == R_REMOVIDO) {
			continue;
		}
		I_push(&reg, index);
		header.quantidadePessoas++;
	}

	for(i = 0; i < N; i++) {
		free(substituicoes[i]);
	}
	free(substituicoes);
	free(criterios);

	/* Torna consistente */
	header.status = STATUS_CONSISTENTE;
	R_writeH(&header, fStream);

	I_write(index, indexPath);
	I_destroy(index);
	fclose(fStream);
	binarioNaTela1(dataPath, indexPath);
}

void f6_readCsv(char *csvPath, char *dataPath) {
	FILE *csvStream, *fStream;
	REG2_HEADER header;
	REG2_DATA reg;

	if((csvStream = fopen(csvPath, "r")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	if((fStream = fopen(dataPath, "wb")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(csvStream);
		return;
	}

	header.status = STATUS_INCONSISTENTE;
	header.quantidadeSeguidores = 0;
	R2_writeH(&header, fStream);

	fscanf(csvStream, "%*[^\r\n]"); /* Pular primeira linha. */
	while(fscanf(csvStream, "%d%*c%d%*c%[^,]%*c%[^,]%*c%s", &reg.idPessoaQueSegue, &reg.idPessoaQueESeguida, reg.grauAmizade, reg.dataInicioQueSegue, reg.dataFimQueSegue) == 5) {
		R2_write(&reg, fStream);
		header.quantidadeSeguidores++;
	}

	header.status = STATUS_CONSISTENTE;
	R2_writeH(&header, fStream);
	fclose(csvStream);
	fclose(fStream);

	binarioNaTela2(dataPath);
}

void f7_sort(char *dataPath, char *sortedDataPath) {
	FILE *fStream, *sortedStream;
	REG2_HEADER header;
	REG2_DATA *reg;
	int i;

	if((fStream = fopen(dataPath, "rb")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	if(R2_readH(&header, fStream) == 0 || header.status == STATUS_INCONSISTENTE) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	reg = NULL;
	if((header.quantidadeSeguidores > 0) && (reg = (REG2_DATA *) malloc(sizeof(REG2_DATA) * header.quantidadeSeguidores)) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	if((sortedStream = fopen(sortedDataPath, "wb")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(fStream);
		return;
	}

	header.status = STATUS_INCONSISTENTE;
	R2_writeH(&header, sortedStream);

	fseek(fStream, 32, SEEK_SET);
	for(i = 0; R2_read(&reg[i], fStream) == 1; ) {
		if(reg[i].removed == R_NAO_REMOVIDO) {
			i += 1;
		}
	}
	fclose(fStream);

	qsort(reg, header.quantidadeSeguidores, sizeof(REG2_DATA), (int (*)(const void *, const void *)) ordena_comparador);

	fseek(sortedStream, 32, SEEK_SET);
	for(i = 0; i < header.quantidadeSeguidores; i++) {
		R2_write(&reg[i], sortedStream);
	}

	header.status = STATUS_CONSISTENTE;
	R2_writeH(&header, sortedStream);
	fclose(sortedStream);

	binarioNaTela2(sortedDataPath);
}

void f8_listMatch(char *f1Path, char *f1IndPath, char *f2Path, char *key) {
	FILE *f1Stream, *f2Stream;
	IND_DATA *indFound1;
	REG2_HEADER h2;
	REG_HEADER h1;
	REG2_DATA aux;
	REG_DATA reg;
	IND_T *ind1;

	if((f1Stream = fopen(f1Path, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	if(R_readH(&h1, f1Stream) == 0 || h1.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(f1Stream);
		return;
	}

	if((f2Stream = fopen(f2Path, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(f1Stream);
		return;
	}

	if(R2_readH(&h2, f2Stream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((ind1 = I_init(f1IndPath)) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((indFound1 = I_find(atoi(key), ind1)) == NULL || fseek(f1Stream, 64 + 64 * indFound1->RRN, SEEK_SET) != 0 || R_read(&reg, f1Stream) != 1 || reg.removed == R_REMOVIDO) {
		printf("Registro inexistente.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		I_destroy(ind1);
		return;
	}

	R_print(&reg);
	while(R2_read(&aux, f2Stream) == 1) {
		if(aux.removed == R_REMOVIDO || aux.idPessoaQueSegue != reg.idPessoa) {
			continue;
		}
		R2_print(&aux);
	}

	fclose(f1Stream);
	fclose(f2Stream);
	I_destroy(ind1);
}

void f9_graph(char *f1Path, char *f1IndPath, char *f2Path) {
	FILE *f1Stream, *f2Stream;
	OBJTOINT *O, *aux;
	GRAFO_ID grafoId;
	REG2_HEADER h2;
	REG_HEADER h1;
	REG2_DATA reg2;
	REG_DATA reg1;
	IND_T *index1;
	char **grafo;
	int i, j;

	if((index1 = I_init(f1IndPath)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}
	I_destroy(index1);

	if((f1Stream = fopen(f1Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}

	if(R_readH(&h1, f1Stream) == 0 || h1.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if((f2Stream = fopen(f2Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if(R2_readH(&h2, f2Stream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((O = OBJ_Init(sizeof(char) * 100, (int (*)(const void *, const void *)) strcmp)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((aux = OBJ_Init(sizeof(GRAFO_ID), (int (*)(const void *, const void *)) grafo_comparador)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		OBJ_Destroy(O);
		return;
	}

	while(R_read(&reg1, f1Stream) == 1) {
		if(reg1.removed == R_REMOVIDO || reg1.nomePessoa[0] == '\0') {
			continue;
		}
		strcpy(grafoId.nomePessoa, reg1.nomePessoa);
		grafoId.idPessoa = reg1.idPessoa;
		OBJ_Push(&grafoId, aux);
		OBJ_Push(reg1.nomePessoa, O);
	}

	grafo = (char **) malloc(sizeof(char *) * OBJ_Count(O));
	for(i = 0; i < OBJ_Count(O); i++) {
		grafo[i] = (char *) malloc(sizeof(char) * OBJ_Count(O));
		memset(grafo[i], 0, sizeof(char) * OBJ_Count(O));
	}

	OBJ_BuildVector(O);
	OBJ_BuildVector(aux);
	while(R2_read(&reg2, f2Stream) == 1) {
		if(reg2.removed == R_REMOVIDO) {
			continue;
		}
		grafoId.idPessoa = reg2.idPessoaQueSegue;
		i = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(i, &grafoId, aux);
		i = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafoId.idPessoa = reg2.idPessoaQueESeguida;
		j = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(j, &grafoId, aux);
		j = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafo[i][j] = 1;
	}

	fclose(f1Stream);
	fclose(f2Stream);
	OBJ_Destroy(aux);

	for(i = 0; i < OBJ_Count(O); i++) {
		OBJ_ObjectFor(i, grafoId.nomePessoa, O);
		printf("%s", grafoId.nomePessoa);
		for(j = 0; j < OBJ_Count(O); j++) {
			if(grafo[i][j] != 1) {
				continue;
			}
			OBJ_ObjectFor(j, grafoId.nomePessoa, O);
			printf(", %s", grafoId.nomePessoa);
		}
		printf("\n");
	}

	OBJ_Destroy(O);
}

void f10_graphInverted(char *f1Path, char *f1IndPath, char *f2Path) {
	FILE *f1Stream, *f2Stream;
	OBJTOINT *O, *aux;
	GRAFO_ID grafoId;
	REG2_HEADER h2;
	REG_HEADER h1;
	REG2_DATA reg2;
	REG_DATA reg1;
	IND_T *index1;
	char **grafo;
	int i, j;

	if((index1 = I_init(f1IndPath)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}
	I_destroy(index1);

	if((f1Stream = fopen(f1Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}

	if(R_readH(&h1, f1Stream) == 0 || h1.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if((f2Stream = fopen(f2Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if(R2_readH(&h2, f2Stream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((O = OBJ_Init(sizeof(char) * 100, (int (*)(const void *, const void *)) strcmp)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((aux = OBJ_Init(sizeof(GRAFO_ID), (int (*)(const void *, const void *)) grafo_comparador)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		OBJ_Destroy(O);
		return;
	}

	while(R_read(&reg1, f1Stream) == 1) {
		if(reg1.removed == R_REMOVIDO || reg1.nomePessoa[0] == '\0') {
			continue;
		}
		strcpy(grafoId.nomePessoa, reg1.nomePessoa);
		grafoId.idPessoa = reg1.idPessoa;
		OBJ_Push(&grafoId, aux);
		OBJ_Push(reg1.nomePessoa, O);
	}

	grafo = (char **) malloc(sizeof(char *) * OBJ_Count(O));
	for(i = 0; i < OBJ_Count(O); i++) {
		grafo[i] = (char *) malloc(sizeof(char) * OBJ_Count(O));
		memset(grafo[i], 0, sizeof(char) * OBJ_Count(O));
	}

	OBJ_BuildVector(O);
	OBJ_BuildVector(aux);
	while(R2_read(&reg2, f2Stream) == 1) {
		if(reg2.removed == R_REMOVIDO) {
			continue;
		}
		grafoId.idPessoa = reg2.idPessoaQueSegue;
		i = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(i, &grafoId, aux);
		i = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafoId.idPessoa = reg2.idPessoaQueESeguida;
		j = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(j, &grafoId, aux);
		j = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafo[j][i] = 1;
	}

	fclose(f1Stream);
	fclose(f2Stream);
	OBJ_Destroy(aux);

	for(i = 0; i < OBJ_Count(O); i++) {
		OBJ_ObjectFor(i, grafoId.nomePessoa, O);
		printf("%s", grafoId.nomePessoa);
		for(j = 0; j < OBJ_Count(O); j++) {
			if(grafo[i][j] != 1) {
				continue;
			}
			OBJ_ObjectFor(j, grafoId.nomePessoa, O);
			printf(", %s", grafoId.nomePessoa);
		}
		printf("\n");
	}

	OBJ_Destroy(O);
}

void f11_bfs(char *f1Path, char *f1IndPath, char *f2Path, char *key) {
	FILE *f1Stream, *f2Stream;
	OBJTOINT *O, *aux;
	GRAFO_ID grafoId;
	REG2_HEADER h2;
	REG_HEADER h1;
	REG2_DATA reg2;
	REG_DATA reg1;
	IND_T *index1;
	char **grafo;
	DEQUE *d;
	int *ant;
	int i, j;

	if((index1 = I_init(f1IndPath)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}
	I_destroy(index1);

	if((f1Stream = fopen(f1Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}

	if(R_readH(&h1, f1Stream) == 0 || h1.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if((f2Stream = fopen(f2Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if(R2_readH(&h2, f2Stream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((O = OBJ_Init(sizeof(char) * 100, (int (*)(const void *, const void *)) strcmp)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((aux = OBJ_Init(sizeof(GRAFO_ID), (int (*)(const void *, const void *)) grafo_comparador)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		OBJ_Destroy(O);
		return;
	}

	while(R_read(&reg1, f1Stream) == 1) {
		if(reg1.removed == R_REMOVIDO || reg1.nomePessoa[0] == '\0') {
			continue;
		}
		strcpy(grafoId.nomePessoa, reg1.nomePessoa);
		grafoId.idPessoa = reg1.idPessoa;
		OBJ_Push(&grafoId, aux);
		OBJ_Push(reg1.nomePessoa, O);
	}

	grafo = (char **) malloc(sizeof(char *) * OBJ_Count(O));
	ant = (int *) malloc(sizeof(int) * OBJ_Count(O));
	for(i = 0; i < OBJ_Count(O); i++) {
		grafo[i] = (char *) malloc(sizeof(char) * OBJ_Count(O));
		memset(grafo[i], 0, sizeof(char) * OBJ_Count(O));
	}
	memset(ant, -1, sizeof(int) * OBJ_Count(O));

	OBJ_BuildVector(O);
	OBJ_BuildVector(aux);
	while(R2_read(&reg2, f2Stream) == 1) {
		if(reg2.removed == R_REMOVIDO) {
			continue;
		}
		grafoId.idPessoa = reg2.idPessoaQueSegue;
		i = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(i, &grafoId, aux);
		i = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafoId.idPessoa = reg2.idPessoaQueESeguida;
		j = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(j, &grafoId, aux);
		j = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafo[j][i] = 1;
	}

	fclose(f1Stream);
	fclose(f2Stream);
	OBJ_Destroy(aux);

	if((d = DQ_New(sizeof(int))) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		OBJ_Destroy(O);
		return;
	}

	i = OBJ_IndexFor(key, O);
	if(i < 0) {
		printf("Falha na execução da funcionalidade.\n");
		OBJ_Destroy(O);
		DQ_Destroy(d);
		return;
	}

	DQ_PushBack(&i, d);
	while(DQ_Size(d) > 0) {
		DQ_ShiftFront(&i, d);
		for(j = 0; j < OBJ_Count(O); j++) {
			if(grafo[i][j] == 1 && ant[j] < 0) {
				ant[j] = i;
				DQ_PushBack(&j, d);
			}
		}
	}

	for(i = 0; i < OBJ_Count(O); i++) {
		if(i == OBJ_IndexFor(key, O)) {
			continue;
		}
		OBJ_ObjectFor(i, grafoId.nomePessoa, O);
		printf("%s", grafoId.nomePessoa);
		if(ant[i] < 0) {
			printf(", NAO SEGUE A CELEBRIDADE");
		} else {
			for(j = ant[i]; j >= 0 && j != OBJ_IndexFor(key, O); j = ant[j]) {
				OBJ_ObjectFor(j, grafoId.nomePessoa, O);
				printf(", %s", grafoId.nomePessoa);
			}
			printf(", %s", key);
		}
		printf("\n");
	}

	OBJ_Destroy(O);
	DQ_Destroy(d);
}

void f12_dfsCycle(char *f1Path, char *f1IndPath, char *f2Path, char *key) {
	FILE *f1Stream, *f2Stream;
	OBJTOINT *O, *aux;
	GRAFO_ID grafoId;
	REG2_HEADER h2;
	REG_HEADER h1;
	REG2_DATA reg2;
	REG_DATA reg1;
	IND_T *index1;
	char **grafo;
	DEQUE *d;
	int *ant;
	int i, j;

	if((index1 = I_init(f1IndPath)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}
	I_destroy(index1);

	if((f1Stream = fopen(f1Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}

	if(R_readH(&h1, f1Stream) == 0 || h1.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if((f2Stream = fopen(f2Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		return;
	}

	if(R2_readH(&h2, f2Stream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((O = OBJ_Init(sizeof(char) * 100, (int (*)(const void *, const void *)) strcmp)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		return;
	}

	if((aux = OBJ_Init(sizeof(GRAFO_ID), (int (*)(const void *, const void *)) grafo_comparador)) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(f1Stream);
		fclose(f2Stream);
		OBJ_Destroy(O);
		return;
	}

	while(R_read(&reg1, f1Stream) == 1) {
		if(reg1.removed == R_REMOVIDO || reg1.nomePessoa[0] == '\0') {
			continue;
		}
		strcpy(grafoId.nomePessoa, reg1.nomePessoa);
		grafoId.idPessoa = reg1.idPessoa;
		OBJ_Push(&grafoId, aux);
		OBJ_Push(reg1.nomePessoa, O);
	}

	grafo = (char **) malloc(sizeof(char *) * OBJ_Count(O));
	ant = (int *) malloc(sizeof(int) * OBJ_Count(O));
	for(i = 0; i < OBJ_Count(O); i++) {
		grafo[i] = (char *) malloc(sizeof(char) * OBJ_Count(O));
		memset(grafo[i], 0, sizeof(char) * OBJ_Count(O));
	}
	memset(ant, -1, sizeof(int) * OBJ_Count(O));

	OBJ_BuildVector(O);
	OBJ_BuildVector(aux);
	while(R2_read(&reg2, f2Stream) == 1) {
		if(reg2.removed == R_REMOVIDO) {
			continue;
		}
		grafoId.idPessoa = reg2.idPessoaQueSegue;
		i = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(i, &grafoId, aux);
		i = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafoId.idPessoa = reg2.idPessoaQueESeguida;
		j = OBJ_IndexFor(&grafoId, aux);
		OBJ_ObjectFor(j, &grafoId, aux);
		j = OBJ_IndexFor(grafoId.nomePessoa, O);
		grafo[j][i] = 1;
	}

	fclose(f1Stream);
	fclose(f2Stream);
	OBJ_Destroy(aux);

	if((d = DQ_New(sizeof(int))) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		OBJ_Destroy(O);
		return;
	}

	i = OBJ_IndexFor(key, O);
	if(i < 0) {
		printf("Falha na execução da funcionalidade.\n");
		OBJ_Destroy(O);
		DQ_Destroy(d);
		return;
	}

	DQ_PushBack(&i, d);
	while(DQ_Size(d) > 0) {
		DQ_ShiftBack(&i, d);
		for(j = OBJ_Count(O) - 1; j >= 0; j--) {
			if(grafo[i][j] == 1 && ant[j] < 0) {
				ant[j] = i;
				DQ_PushBack(&j, d);
			}
		}
	}
	DQ_Destroy(d);

	i = OBJ_IndexFor(key, O);
	if(ant[i] < 0) {
		printf("A FOFOCA NAO RETORNOU\n");
	} else {
		for(i = ant[i], j = 1; i != OBJ_IndexFor(key, O) && ant[i] >= 0; j++) {
			i = ant[i];
		}
		printf("%d\n", j);
	}

	OBJ_Destroy(O);
}



void fx_invalidadeStatus(char *filePath) {
	FILE *fStream;
	char aux;

	if((fStream = fopen(filePath, "rb+")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	aux = STATUS_INCONSISTENTE;
	if(fwrite(&aux, sizeof(char), 1, fStream) != 1) {
		printf("Falha no processamento do arquivo.\n");
	} else {
		printf("Invalidado com sucesso!\n");
	}

	fclose(fStream);
}

void fx_compressFile2(char *file2Path) {
	FILE *fStream, *newFStream;
	REG2_HEADER h2;
	REG2_DATA reg2;
	char buff[512];
	long n;

	if((newFStream = tmpfile()) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		return;
	}

	if((fStream = fopen(file2Path, "rb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(newFStream);
		return;
	}

	if(R2_readH(&h2, fStream) == 0 || h2.status == STATUS_INCONSISTENTE) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(fStream);
		fclose(newFStream);
		return;
	}

	R2_writeH(&h2, newFStream);
	while(R2_read(&reg2, fStream) == 1) {
		if(reg2.removed == R_REMOVIDO) {
			continue;
		}
		R2_write(&reg2, newFStream);
	}

	fclose(fStream);
	fseek(newFStream, 0, SEEK_SET);
	if((fStream = fopen(file2Path, "wb")) == NULL) {
		printf("Falha na execução da funcionalidade.\n");
		fclose(newFStream);
		return;
	}

	while((n = fread(buff, sizeof(char), 512, newFStream)) > 0) {
		fwrite(buff, sizeof(char), n, fStream);
	}

	fclose(fStream);
	fclose(newFStream);
}


