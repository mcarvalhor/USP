#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <consts.h>
#include <buffer.h>
#include <register.h>
#include <index.h>
#include <bindex.h>
#include <escreverTela.h>
#include <vector_builder.h>
#include <merge_sort.h>


typedef struct {
	char campo[1000];
	char valor[1000];
	char campoMod[1000];
	char valorMod[1000];
} TO_BE_MODIFIED_FIELD;

typedef struct {
	long byteOffset;
	long nextByteOffset;
	int size;
	int level;
} REMOVED_LIST_NODE;



int node_cmps(REMOVED_LIST_NODE *A, REMOVED_LIST_NODE *B) {
	return B->size - A->size;
}

int node_cmpb(REMOVED_LIST_NODE *A, REMOVED_LIST_NODE *B) {
	return B->byteOffset - A->byteOffset;
}

int node_cmpbr(REMOVED_LIST_NODE *A, REMOVED_LIST_NODE *B) {
	return A->byteOffset - B->byteOffset;
}

int node_cmplr(REMOVED_LIST_NODE *A, REMOVED_LIST_NODE *B) {
	return A->level - B->level;
}

int regData_cmp(REG_DATA *A, REG_DATA *B) {
	return B->idServidor - A->idServidor;
}

int field_isMarked(REG_DATA *aux, unsigned long n, unsigned long i, TO_BE_MODIFIED_FIELD *fields) {
	for(; i < n; i++) {
		if(strcmp(fields[i].campo, "idServidor") == 0) {
			if(atoi(fields[i].valor) == aux->idServidor) {
				return i;
			}
		} else if(strcmp(fields[i].campo, "salarioServidor") == 0) {
			if((fields[i].valor[0] == '\0' && aux->salarioServidor < 0 ) || atof(fields[i].valor) == aux->salarioServidor) {
				return i;
			}
		} else if(strcmp(fields[i].campo, "telefoneServidor") == 0) {
			if(strncmp(fields[i].valor, aux->telefoneServidor, 14) == 0) {
				return i;
			}
		} else if(strcmp(fields[i].campo, "nomeServidor") == 0) {
			if(strcmp(fields[i].valor, aux->nomeServidor) == 0) {
				return i;
			}
		} else if(strcmp(fields[i].campo, "cargoServidor") == 0) {
			if(strcmp(fields[i].valor, aux->cargoServidor) == 0) {
				return i;
			}
		}
	}
	return -1;
}

int f1_csvBuild(char *binPath, char *csvPath) {
	FILE *csvStream, *binStream;
	long ultimoSize;
	REG_HEADER auxH;
	REG_DATA aux;
	int i, auxN;
	
	csvStream = fopen(csvPath, "r");
	if(!csvStream) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	binStream = fopen(binPath, "w+b");
	if(!binStream) {
		fclose(csvStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}


	auxH.status = STATUS_INCONSISTENTE;
	auxH.first = -1;
	fscanf(csvStream, "%[^,]%*c%[^,]%*c%[^,]%*c%[^,]%*c%[^\r\n]", auxH.desCampos[0], auxH.desCampos[1], auxH.desCampos[2], auxH.desCampos[3], auxH.desCampos[4]);
	R_writeH(&auxH, binStream);

	ultimoSize = -1;
	while(fscanf(csvStream, "%d%*c%lf%*c", &aux.idServidor, &aux.salarioServidor) == 2) {
		memset(aux.telefoneServidor, LIXO, sizeof(char) * 14);
		if(fscanf(csvStream, "%[^,]", aux.telefoneServidor) != 1) {
			aux.telefoneServidor[0] = '\0';
		}
		if(fscanf(csvStream, "%*c%[^,]", aux.nomeServidor) != 1) {
			aux.nomeServidor[0] = '\0';
		}
		if(fscanf(csvStream, "%*c%[^\r\n]", aux.cargoServidor) != 1) {
			aux.cargoServidor[0] = '\0';
		}
		R_write(&aux, binStream, &ultimoSize);
	}

	// COMENTADO PQ: não faz padding mais na última página.
	//R_writePad(binStream);

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, binStream);

	fclose(csvStream);
	fclose(binStream);

	return 1;
}

int f2_list(char *binPath) {
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	char found;
	int i;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	found = 0;
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		found = 1;
		/*int rnd = rand() % 10;
		switch(rnd) {
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
				printf("%s %d\n", "idServidor", aux.idServidor);
			break;
			case 6:
				if(aux.salarioServidor < 0) {
					printf("%s %s\n", "salarioServidor", "NULO");
				} else {
					printf("%s %lf\n", "salarioServidor", aux.salarioServidor);
				}
			break;
			case 7:
				if(aux.telefoneServidor[0] == '\0') {
					printf("%s %s\n", "telefoneServidor", "NULO");
				} else {
					printf("%s \"%.14s\"\n", "telefoneServidor", aux.telefoneServidor);
				}
			break;
			case 8:
				if(aux.nomeServidor[0] == '\0') {
					printf("%s %s\n", "nomeServidor", "NULO");
				} else {
					printf("%s \"%s\"\n", "nomeServidor", aux.nomeServidor);
				}
			break;
			case 9:
				if(aux.cargoServidor[0] == '\0') {
					printf("%s %s\n", "cargoServidor", "NULO");
				} else {
					printf("%s \"%s\"\n", "cargoServidor", aux.cargoServidor);
				}
			break;
		}
		continue;/**/
		printf("%d ", aux.idServidor);
		if(aux.salarioServidor >= 0) {
			printf("%.2lf", aux.salarioServidor);
		} else {
			for(i = 0; i < 8; i++) {
				printf(" ");
			}
		}
		if(strncmp(aux.telefoneServidor, "", 14) != 0) {
			printf(" %.14s", aux.telefoneServidor);
		} else {
			printf(" ");
			for(i = 0; i < 14; i++) {
				printf(" ");
			}
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			printf(" %ld %s", strlen(aux.nomeServidor), aux.nomeServidor);
		}
		if(strcmp(aux.cargoServidor, "") != 0) {
			printf(" %ld %s", strlen(aux.cargoServidor), aux.cargoServidor);
		}
		printf("\n");
	}

	if(!found) {
		printf("Registro inexistente.\n");
	} else {
		printf("Número de páginas de disco acessadas: %ld\n", B_count(stream));
	}
	fclose(stream);
	return 1;
}

int f3_search(char *binPath, char *field, char *value) {
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	char found;
	double k;
	int i, j;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	found = 0;
	if(strcmp(field, "idServidor") == 0) { // idServidor
		j = atoi(value);
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO || aux.idServidor != j) {
				continue;
			}
			found = 1;
			printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
			if(aux.salarioServidor >= 0) {
				printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
			}
			if(strncmp(aux.telefoneServidor, "", 14) != 0) {
				printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
			}
			if(strcmp(aux.nomeServidor, "") != 0) {
				printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
			}
			if(strcmp(aux.cargoServidor, "") != 0) {
				printf("%s\n", aux.cargoServidor);
			} else {
				printf("valor nao declarado\n");
			}
			printf("\n");
			break;
		}
	} else if(strcmp(field, "salarioServidor") == 0) { // salarioServidor
		k = (double) atof(value);
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO || aux.salarioServidor != k) {
				continue;
			}
			found = 1;
			printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
			if(aux.salarioServidor >= 0) {
				printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
			}
			if(strncmp(aux.telefoneServidor, "", 14) != 0) {
				printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
			}
			if(strcmp(aux.nomeServidor, "") != 0) {
				printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
			}
			if(strcmp(aux.cargoServidor, "") != 0) {
				printf("%s\n", aux.cargoServidor);
			} else {
				printf("valor nao declarado\n");
			}
			printf("\n");
		}
	} else if(strcmp(field, "telefoneServidor") == 0) { // telefoneServidor
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO || strncmp(aux.telefoneServidor, value, 14) != 0) {
				continue;
			}
			found = 1;
			printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
			if(aux.salarioServidor >= 0) {
				printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
			}
			if(strncmp(aux.telefoneServidor, "", 14) != 0) {
				printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
			}
			if(strcmp(aux.nomeServidor, "") != 0) {
				printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
			}
			if(strcmp(aux.cargoServidor, "") != 0) {
				printf("%s\n", aux.cargoServidor);
			} else {
				printf("valor nao declarado\n");
			}
			printf("\n");
		}
	} else if(strcmp(field, "nomeServidor") == 0) { // nomeServidor
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO || strcmp(aux.nomeServidor, value) != 0) {
				continue;
			}
			found = 1;
			printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
			if(aux.salarioServidor >= 0) {
				printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
			}
			if(strncmp(aux.telefoneServidor, "", 14) != 0) {
				printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
			}
			if(strcmp(aux.nomeServidor, "") != 0) {
				printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
			}
			if(strcmp(aux.cargoServidor, "") != 0) {
				printf("%s\n", aux.cargoServidor);
			} else {
				printf("valor nao declarado\n");
			}
			printf("\n");
		}
	} else if(strcmp(field, "cargoServidor") == 0) { // cargoServidor
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO || strcmp(aux.cargoServidor, value) != 0) {
				continue;
			}
			found = 1;
			printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
			if(aux.salarioServidor >= 0) {
				printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
			}
			if(strncmp(aux.telefoneServidor, "", 14) != 0) {
				printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
			}
			if(strcmp(aux.nomeServidor, "") != 0) {
				printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
			} else {
				printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
			}
			if(strcmp(aux.cargoServidor, "") != 0) {
				printf("%s\n", aux.cargoServidor);
			} else {
				printf("valor nao declarado\n");
			}
			printf("\n");
		}
	} else {
		printf("Campo buscado inexistente.\n");
	}

	if(!found) {
		printf("Registro inexistente.\n");
	} else {
		printf("Número de páginas de disco acessadas: %ld\n", B_count(stream));
	}

	fclose(stream);
	return 1;
}

int f4_removeRegs(char *binPath, int n, FILE *input) {
	char found, field[5000], value[5000];
	int i, j, nRemoved;
	TO_BE_MODIFIED_FIELD *fields;
	REMOVED_LIST_NODE *removed, removedAux;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	long offset;
	double k;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	// Ler todos os campos de análise de stdin
	builder = V_new(sizeof(TO_BE_MODIFIED_FIELD));
	fields = (TO_BE_MODIFIED_FIELD *) malloc(sizeof(TO_BE_MODIFIED_FIELD));
	for(i = 0; i < n; i++) {
		scanf("%s", fields->campo);
		scan_quote_string(fields->valor);
		V_pushback(fields, builder);
	}
	free(fields);
	n = V_build(&fields, builder);
	V_destroy(builder);

	// Ler todos os registros pra ver quais serão removidos
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	builder = V_new(sizeof(REMOVED_LIST_NODE));
	found = 0;
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		if((removed->level=field_isMarked(&aux, n, 0, fields)) < 0) {
			continue;
		}
		found = 1;
		removed->level++;
		removed->byteOffset = aux.byteOffset;
		removed->size = aux.size;
		V_pushback(removed, builder);
	}
	free(fields);
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);

	if(nRemoved > 0) {
		// Ordenar crescentemente por level e depois por tamanho
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpbr);
		builder = V_new(sizeof(REMOVED_LIST_NODE));
		offset = auxH.first;
		while(offset != L_NIL) {
			fseek(stream, offset, SEEK_SET);
			R_read(&aux, stream);
			removedAux.nextByteOffset = aux.next;
			removedAux.byteOffset = offset;
			removedAux.size = aux.size;
			removedAux.level = 0;
			V_pushback(&removedAux, builder);
			offset = aux.next;
		}
		removed = (REMOVED_LIST_NODE *) realloc(removed, sizeof(REMOVED_LIST_NODE) * (nRemoved + V_size(builder)));
		V_copy(&removed[nRemoved], builder);
		nRemoved += V_size(builder);
		V_destroy(builder);
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmplr);
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmps);
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	free(removed);
	fclose(stream);
	return 1;
}

int f5_addRegs(char *binPath, int n, FILE *input) {
	char found, field[5000];
	long offset;
	int i, j, nRemoved;
	REMOVED_LIST_NODE *removed;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux, *toBeAdded;
	double k;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	// Ler lista de removidos
	builder = V_new(sizeof(REMOVED_LIST_NODE));
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	offset = auxH.first;
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		removed->nextByteOffset = aux.next;
		removed->byteOffset = offset;
		removed->size = aux.size;
		V_pushback(removed, builder);
		offset = aux.next;
	}
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);

	// Adiciona um por um: tenta na lista de removidos da RAM. Caso não dê: adiciona em lista na RAM pra adicionar ao fim.
	builder = V_new(sizeof(REG_DATA));
	aux.removed = R_NAO_REMOVIDO;
	for(i = 0; i < n; i++) {
		scanf("%d", &aux.idServidor);
		scan_quote_string(field);
		if(field[0] == '\0') {
			aux.salarioServidor = -1;
		} else {
			aux.salarioServidor = atof(field);
		}
		scan_quote_string(field);
		strncpy(aux.telefoneServidor, field, 14);
		scan_quote_string(aux.nomeServidor);
		scan_quote_string(aux.cargoServidor);
		aux.size = R_calculateSize(&aux);
		for(j = 0; j < nRemoved; j++) {
			if(removed[j].size >= aux.size) {
				break;
			}
		}
		if(j >= nRemoved) { // não cabe em nenhum
			V_pushback(&aux, builder);
			continue;
		}
		aux.size = removed[j].size;
		aux.byteOffset = removed[j].byteOffset;
		R_rewriteBack(&aux, stream);
		memmove(&removed[j], &removed[j + 1], sizeof(REMOVED_LIST_NODE) * (nRemoved - j - 1));
		nRemoved--;
	}
	n = V_build(&toBeAdded, builder);
	V_destroy(builder);

	// Encontrar fim do arquivo.
	fseek(stream, 0, SEEK_END);
	offset = ftell(stream);
	if(offset % 32000 == 0) {
		offset = -1;
	} else {
		while(R_read(&aux, stream) == 1) {
			offset = aux.size;
		}
	}

	// Adicionar os que faltam
	for(i = 0; i < n; i++) {
		R_write(&toBeAdded[i], stream, &offset);
	}

	// Atualizar lista
	if(nRemoved > 0) {
		// Não ordena: pra manter ordem original.
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	} else {
		auxH.first = L_NIL;
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	fclose(stream);
	return 1;
}

int f6_updateRegsSlowly_remove(REG_DATA *reg, REG_HEADER *header, FILE *stream) {
	REMOVED_LIST_NODE *removed;
	VECTOR_T *builder;
	REG_DATA aux;
	int nRemoved;
	long offset;

	builder = V_new(sizeof(REMOVED_LIST_NODE));
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	offset = header->first;
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		removed->nextByteOffset = aux.next;
		removed->byteOffset = offset;
		removed->size = aux.size;
		removed->level = 0;
		V_pushback(removed, builder);
		offset = aux.next;
	}
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);
}

int f6_updateRegsSlowly(char *binPath, int n, FILE *input) {
	char found, rewrite, field[5000];
	long offset, seekbackByteoffset, lastOffset, lastSize, lastI;
	int i, j, k, nRemoved, nUpdated, level;
	TO_BE_MODIFIED_FIELD auxF;
	REMOVED_LIST_NODE *removed;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux, auxR, auxT, original, *updated;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	/*builder = V_new(sizeof(REMOVED_LIST_NODE));
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	offset = auxH.first;
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		removed->nextByteOffset = aux.next;
		removed->byteOffset = offset;
		removed->size = aux.size;
		removed->level = 0;
		V_pushback(removed, builder);
		offset = aux.next;
	}
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);*/

	lastSize = -1;
	for(i = 0; i < n; i++) {
		scanf("%s", auxF.campo);
		scan_quote_string(auxF.valor);
		scanf("%s", auxF.campoMod);
		scan_quote_string(auxF.valorMod);
		fseek(stream, PAGE_SIZE, SEEK_SET);
		while(R_read(&aux, stream) == 1) {
			if(aux.removed == R_REMOVIDO) { // Removido: ignorar
				continue;
			}
			if(field_isMarked(&aux, 1, 0, &auxF) < 0) {
				continue;
			}
			if(strcmp(auxF.campoMod, "idServidor") == 0) {
				aux.idServidor = atoi(auxF.valorMod);
			} else if(strcmp(auxF.campoMod, "salarioServidor") == 0) {
				if(auxF.valorMod[0] == '\0') {
					aux.salarioServidor = -1;
				} else {
					aux.salarioServidor = atof(auxF.valorMod);
				}
			} else if(strcmp(auxF.campoMod, "telefoneServidor") == 0) {
				strncpy(aux.telefoneServidor, auxF.valorMod, 14);
			} else if(strcmp(auxF.campoMod, "nomeServidor") == 0) {
				strcpy(aux.nomeServidor, auxF.valorMod);
			} else if(strcmp(auxF.campoMod, "cargoServidor") == 0) {
				strcpy(aux.cargoServidor, auxF.valorMod);
			}
			memcpy(&original, &aux, sizeof(REG_DATA));
			// AQUI VAI COMEÇAR REINSERÇÃO OU REESCRITA
			if(aux.size >= R_calculateSize(&aux)) { // Coube. Continua ali.
				R_rewriteBack(&aux, stream);
				continue;
			}
			/*// Não coube. Ver na lista de removidos ou inserir ao fim.
			removed = (REMOVED_LIST_NODE *) realloc(removed, sizeof(REMOVED_LIST_NODE) * (nRemoved + 1));
			for(j = 0; j < nRemoved; j++) {
				if(removed[j].size >= aux.size) {
					break;
				}
			}
			memmove(&removed[j + 1], &removed[j], sizeof(REMOVED_LIST_NODE) * (nRemoved - j));
			removed[j].byteOffset = aux.byteOffset;
			removed[j].level = i + 1;
			removed[j].size = aux.size;
			nRemoved++;
			for(; j < nRemoved; j++) {
				if(removed[j].size >= R_calculateSize(&aux)) {
					break;
				}
			}
			if(j >= nRemoved) { // não cabe em nenhum
				aux.size = R_calculateSize(&aux);
				seekbackByteoffset = ftell(stream);
				fseek(stream, 0, SEEK_END);
				R_write(&aux, stream, &lastSize);
				fseek(stream, seekbackByteoffset, SEEK_SET);
				continue;
			}
			aux.size = removed[j].size;
			aux.byteOffset = removed[j].byteOffset;
			memmove(&removed[j], &removed[j + 1], sizeof(REMOVED_LIST_NODE) * (nRemoved - j - 1));
			nRemoved--;
			continue;*/
			seekbackByteoffset = ftell(stream);
			lastOffset = L_NIL;
			offset = auxH.first;
			while(offset != L_NIL) {
				fseek(stream, offset, SEEK_SET);
				R_read(&auxR, stream);
				if(auxR.size >= aux.size) {
					break;
				}
				lastOffset = offset;
				offset = auxR.next;
			}
			aux.next = offset;
			aux.removed = R_REMOVIDO;
			R_rewriteBack(&aux, stream);
			if(lastOffset != L_NIL) {
				fseek(stream, lastOffset, SEEK_SET);
				R_read(&auxT, stream);
				auxT.next = aux.byteOffset;
				R_rewriteBack(&auxT, stream);
			} else {
				auxH.first = aux.byteOffset;
			}
			// Inserido na lista de removidos. Agora, procurar se cabe em algum de lá.
			rewrite = 0;
			while(offset != L_NIL) {
				fseek(stream, offset, SEEK_SET);
				R_read(&auxR, stream);
				if(auxR.size >= R_calculateSize(&original)) {
					rewrite = 1;
					break;
				}
				lastOffset = offset;
				offset = auxR.next;
			}
			if(rewrite) { // Achou posição que cabe.
				if(lastOffset != L_NIL) {
					fseek(stream, lastOffset, SEEK_SET);
					R_read(&auxT, stream);
					auxT.next = auxR.next;
					R_rewriteBack(&auxT, stream);
				} else {
					auxH.first = auxR.next;
				}
				original.removed = R_NAO_REMOVIDO;
				original.byteOffset = auxR.byteOffset;
				original.size = auxR.size;
				original.next = -1;
				R_rewriteBack(&original, stream);
			} else { // Escrever ao fim mesmo.
				fseek(stream, 0, SEEK_END);
				original.size = R_calculateSize(&original);
				R_write(&original, stream, &lastSize);
			}
			fseek(stream, seekbackByteoffset, SEEK_SET);
		}
	}

	/*if(nRemoved > 0) {
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	} else {
		auxH.first = L_NIL;
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}*/

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	fclose(stream);
	return 1;
}

int f6_updateRegs(char *binPath, int n, FILE *input) {
	char found, rewrite, field[5000];
	long offset, lastOffset, lastI;
	int i, j, k, nRemoved, nUpdated, level;
	TO_BE_MODIFIED_FIELD auxF, *fields;
	REMOVED_LIST_NODE *removed;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux, original, *updated;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	// Ler todos os campos de análise de stdin
	builder = V_new(sizeof(TO_BE_MODIFIED_FIELD));
	fields = (TO_BE_MODIFIED_FIELD *) malloc(sizeof(TO_BE_MODIFIED_FIELD));
	for(i = 0; i < n; i++) {
		scanf("%s", fields->campo);
		scan_quote_string(fields->valor);
		scanf("%s", fields->campoMod);
		scan_quote_string(fields->valorMod);
		V_pushback(fields, builder);
	}
	free(fields);
	n = V_build(&fields, builder);
	V_destroy(builder);

	// Ler todos os registros pra ver quais já podem ser atualizados no próprio local.
	builder = V_new(sizeof(REG_DATA));
	aux.size = -1;
	found = 0;
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) { // Removido: ignorar
			continue;
		}
		memcpy(&original, &aux, sizeof(REG_DATA));
		rewrite = 0;
		level = -1;
		aux.size = R_calculateSize(&aux);
		while((level = field_isMarked(&aux, n, level + 1, fields)) >= 0) { // Se tiver marcado e enquanto tá cabendo no próprio local...
			found = rewrite = 1;
			if(strcmp(fields[level].campoMod, "idServidor") == 0) {
				aux.idServidor = atoi(fields[level].valorMod);
			} else if(strcmp(fields[level].campoMod, "salarioServidor") == 0) {
				if(fields[level].valorMod[0] == '\0') {
					aux.salarioServidor = -1;
				} else {
					aux.salarioServidor = atof(fields[level].valorMod);
				}
			} else if(strcmp(fields[level].campoMod, "telefoneServidor") == 0) {
				strncpy(aux.telefoneServidor, fields[level].valorMod, 14);
			} else if(strcmp(fields[level].campoMod, "nomeServidor") == 0) {
				strcpy(aux.nomeServidor, fields[level].valorMod);
			} else if(strcmp(fields[level].campoMod, "cargoServidor") == 0) {
				strcpy(aux.cargoServidor, fields[level].valorMod);
			}
			if(aux.size < R_calculateSize(&aux)) {
				V_pushback(&original, builder);
				rewrite = 0;
				break;
			}
		}
		if(rewrite) {
			R_rewriteBack(&aux, stream);
		}
	}
	lastOffset = aux.size;
	lastI = -1;
	nUpdated = V_build(&updated, builder);
	V_destroy(builder);

	// Ler lista de removidos
	builder = V_new(sizeof(REMOVED_LIST_NODE));
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	offset = auxH.first;
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		removed->nextByteOffset = aux.next;
		removed->byteOffset = offset;
		removed->size = aux.size;
		removed->level = 0;
		V_pushback(removed, builder);
		offset = aux.next;
	}
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);

	// Iterar para atualizar os que não couberam antes...
	fseek(stream, 0, SEEK_END);
	for(level = 0; level < n; level++) {
		for(i = 0; i < nUpdated; i++) {
			if(field_isMarked(&updated[i], level + 1, level, fields) >= 0) {
				if(strcmp(fields[level].campoMod, "idServidor") == 0) {
					updated[i].idServidor = atoi(fields[level].valorMod);
				} else if(strcmp(fields[level].campoMod, "salarioServidor") == 0) {
					if(fields[level].valorMod[0] == '\0') {
						updated[i].salarioServidor = -1;
					} else {
						updated[i].salarioServidor = atof(fields[level].valorMod);
					}
				} else if(strcmp(fields[level].campoMod, "telefoneServidor") == 0) {
					strncpy(updated[i].telefoneServidor, fields[level].valorMod, 14);
				} else if(strcmp(fields[level].campoMod, "nomeServidor") == 0) {
					strcpy(updated[i].nomeServidor, fields[level].valorMod);
				} else if(strcmp(fields[level].campoMod, "cargoServidor") == 0) {
					strcpy(updated[i].cargoServidor, fields[level].valorMod);
				}
				if(updated[i].size < R_calculateSize(&updated[i])) {
					removed = (REMOVED_LIST_NODE *) realloc(removed, sizeof(REMOVED_LIST_NODE) * (nRemoved + 1));
					for(j = 0; j < nRemoved; j++) {
						if(removed[j].size >= updated[i].size) {
							break;
						}
					}
					memmove(&removed[j + 1], &removed[j], sizeof(REMOVED_LIST_NODE) * (nRemoved - j));
					removed[j].byteOffset = updated[i].byteOffset;
					removed[j].level = level + 1;
					removed[j].size = updated[i].size;
					nRemoved++;
					for(; j < nRemoved; j++) {
						if(removed[j].size >= R_calculateSize(&updated[i])) {
							break;
						}
					}
					if(j >= nRemoved) { // não cabe em nenhum
						updated[i].size = R_calculateSize(&updated[i]);
						if(R_write(&updated[i], stream, &lastOffset) == 2  && lastI >= 0) {
							for(k = 0; k < nRemoved; k++) {
								if(removed[k].byteOffset == lastI) {
									removed[k].size = PAGE_SIZE - (removed[k].byteOffset % PAGE_SIZE) - sizeof(char) - sizeof(int);
								}
							}
							for(k = 0; k < nUpdated; k++) {
								if(updated[k].byteOffset == lastI) {
									updated[k].size = PAGE_SIZE - (updated[k].byteOffset % PAGE_SIZE) - sizeof(char) - sizeof(int);
								}
							}
						}
						lastI = updated[i].byteOffset;
						lastOffset = updated[i].size;
						continue;
					}
					updated[i].size = removed[j].size;
					updated[i].byteOffset = removed[j].byteOffset;
					memmove(&removed[j], &removed[j + 1], sizeof(REMOVED_LIST_NODE) * (nRemoved - j - 1));
					nRemoved--;
				}
			}
		}
	}

	// Iterar para salvar os atualizados...
	for(i = 0; i < nUpdated; i++) {
		R_rewriteBack(&updated[i], stream);
	}

	if(nRemoved > 0) {
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	} else {
		auxH.first = L_NIL;
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	free(removed);
	free(fields);
	fclose(stream);
	return 1;
}

int f7_sort(char *inPath, char *outPath) {
	FILE *inStream, *outStream;
	long ultimoSize;
	VECTOR_T *builder;
	REG_HEADER auxH;
	REG_DATA aux, *auxV;
	int i, auxN;

	inStream = fopen(inPath, "rb");
	if(!inStream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, inStream);
	if(auxH.status == STATUS_INCONSISTENTE) {
		fclose(inStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	outStream = fopen(outPath, "w+b");
	if(!outStream) {
		fclose(inStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	auxH.status = STATUS_INCONSISTENTE;
	auxH.first = -1;
	R_writeH(&auxH, outStream);

	builder = V_new(sizeof(REG_DATA));
	while(R_read(&aux, inStream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		V_pushback(&aux, builder);
	}
	auxN = V_build(&auxV, builder);
	V_destroy(builder);

	fclose(inStream);

	MS_sort(auxV, auxN, sizeof(REG_DATA), (int (*)(const void *, const void *)) regData_cmp);

	ultimoSize = -1;
	for(i = 0; i < auxN; i++) {
		R_write(&auxV[i], outStream, &ultimoSize);
	}

	// COMENTADO PQ: não faz padding mais na última página.
	//R_writePad(outStream);

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, outStream);

	fclose(outStream);

	return 1;
}

int f8_merge(char *in1Path, char *in2Path, char *outPath) {
	FILE *in1Stream, *in2Stream, *outStream;
	REG_DATA aux1, aux2;
	REG_HEADER auxH;
	long ultimoSize;
	int eof1, eof2;

	in2Stream = fopen(in2Path, "rb");
	if(!in2Stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, in2Stream);
	if(auxH.status == STATUS_INCONSISTENTE) {
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	in1Stream = fopen(in1Path, "rb");
	if(!in1Stream) {
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, in1Stream);
	if(auxH.status == STATUS_INCONSISTENTE) {
		fclose(in1Stream);
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	outStream = fopen(outPath, "wb+");
	if(!outStream) {
		fclose(in1Stream);
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	auxH.status = STATUS_INCONSISTENTE;
	auxH.first = -1;
	R_writeH(&auxH, outStream);

	// Iniciar leitura do primeiro registro
	while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
	while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);

	ultimoSize = -1;
	while(eof1 == 1 && eof2 == 1) {
		if(aux1.idServidor < aux2.idServidor) {
			R_write(&aux1, outStream, &ultimoSize);
			while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
		} else if(aux1.idServidor > aux2.idServidor) {
			R_write(&aux2, outStream, &ultimoSize);
			while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);
		} else { // id1 == id2
			R_write(&aux1, outStream, &ultimoSize);
			while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
			while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);
		}
	}

	if(eof1 == 1) { // Chegou no fim do arquivo 2.
		if(aux1.removed != R_REMOVIDO) {
			R_write(&aux1, outStream, &ultimoSize);
		}
		while(R_read(&aux1, in1Stream) == 1) {
			if(aux1.removed == R_REMOVIDO) {
				continue;
			}
			R_write(&aux1, outStream, &ultimoSize);
		}
	}

	if(eof2 == 1) { // Chegou no fim do arquivo 1.
		if(aux2.removed != R_REMOVIDO) {
			R_write(&aux2, outStream, &ultimoSize);
		}
		while(R_read(&aux2, in2Stream) == 1) {
			if(aux2.removed == R_REMOVIDO) {
				continue;
			}
			R_write(&aux2, outStream, &ultimoSize);
		}
	}

	fclose(in1Stream);
	fclose(in2Stream);

	// COMENTADO PQ: não faz padding mais na última página.
	//R_writePad(outStream);

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, outStream);

	fclose(outStream);

	return 1;
}

int f9_match(char *in1Path, char *in2Path, char *outPath) {
	FILE *in1Stream, *in2Stream, *outStream;
	REG_DATA aux1, aux2;
	REG_HEADER auxH;
	long ultimoSize;
	int eof1, eof2;

	in2Stream = fopen(in2Path, "rb");
	if(!in2Stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, in2Stream);
	if(auxH.status == STATUS_INCONSISTENTE) {
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	in1Stream = fopen(in1Path, "rb");
	if(!in1Stream) {
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, in1Stream);
	if(auxH.status == STATUS_INCONSISTENTE) {
		fclose(in1Stream);
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	outStream = fopen(outPath, "wb+");
	if(!outStream) {
		fclose(in1Stream);
		fclose(in2Stream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	auxH.status = STATUS_INCONSISTENTE;
	auxH.first = -1;
	R_writeH(&auxH, outStream);

	// Iniciar leitura do primeiro registro
	while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
	while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);

	ultimoSize = -1;
	while(eof1 == 1 && eof2 == 1) {
		if(aux1.idServidor < aux2.idServidor) {
			while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
		} else if(aux1.idServidor > aux2.idServidor) {
			while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);
		} else { // id1 == id2
			R_write(&aux1, outStream, &ultimoSize);
			while((eof1 = R_read(&aux1, in1Stream)) == 1 && aux1.removed == R_REMOVIDO);
			while((eof2 = R_read(&aux2, in2Stream)) == 1 && aux2.removed == R_REMOVIDO);
		}
	}

	fclose(in1Stream);
	fclose(in2Stream);

	// COMENTADO PQ: não faz padding mais na última página.
	//R_writePad(outStream);

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, outStream);

	fclose(outStream);

	return 1;
}

int f10_index(char *binPath, char *indPath) {
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	IND_T *index;
	int i;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	index = I_init(NULL);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			I_push(&aux, index);
		}
	}

	fclose(stream);

	if(I_write(index, indPath) != 1) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	return 1;
}

int f11_searchKey(char *binPath, char *indPath, char *value) {
	long lastPage, pageCount;
	REG_HEADER auxH;
	IND_DATA *items;
	FILE *stream;
	REG_DATA aux;
	IND_T *index;
	char found;
	int i, n;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	index = I_init(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	items = I_iterate(&n, value, index);

	found = 0;
	for(i = 0; i < n; i++) {
		fseek(stream, items[i].byteOffset, SEEK_SET);
		if(R_read(&aux, stream) != 1 || aux.removed == R_REMOVIDO) {
			printf("Falha no processamento do arquivo.\n");
			return 0;
		}
		if(strcmp(aux.nomeServidor, value) != 0) {
			continue;
		}
		found = 1;
		printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
		if(aux.salarioServidor >= 0) {
			printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
		}
		if(strncmp(aux.telefoneServidor, "", 14) != 0) {
			printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
		}
		if(strcmp(aux.cargoServidor, "") != 0) {
			printf("%s\n", aux.cargoServidor);
		} else {
			printf("valor nao declarado\n");
		}
		printf("\n");
	}

	pageCount = 1;
	lastPage = -1;
	for(i = 0; i < n; i++) {
		if(items[i].byteOffset/PAGE_SIZE != lastPage) {
			pageCount++;
		}
		lastPage = items[i].byteOffset / PAGE_SIZE;
	}

	if(!found) {
		printf("Registro inexistente.\n");
	} else {
		printf("Número de páginas de disco para carregar o arquivo de índice: %ld\n", I_count(index));
		printf("Número de páginas de disco para acessar o arquivo de dados: %ld\n", pageCount);
	}

	fclose(stream);
	return 1;
}

int f12_removeRegsKey(char *binPath, char *indPath, int n, FILE *input) {
	char found, field[5000], value[5000];
	int i, j, nRemoved;
	TO_BE_MODIFIED_FIELD *fields;
	REMOVED_LIST_NODE *removed, removedAux;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	IND_T *index;
	long offset;
	double k;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	index = I_init(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	// Ler todos os campos de análise de stdin
	builder = V_new(sizeof(TO_BE_MODIFIED_FIELD));
	fields = (TO_BE_MODIFIED_FIELD *) malloc(sizeof(TO_BE_MODIFIED_FIELD));
	for(i = 0; i < n; i++) {
		scanf("%s", fields->campo);
		scan_quote_string(fields->valor);
		V_pushback(fields, builder);
	}
	free(fields);
	n = V_build(&fields, builder);
	V_destroy(builder);

	// Ler todos os registros pra ver quais serão removidos
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	builder = V_new(sizeof(REMOVED_LIST_NODE));
	found = 0;
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		if((removed->level=field_isMarked(&aux, n, 0, fields)) < 0) {
			continue;
		}
		found = 1;
		removed->level++;
		removed->byteOffset = aux.byteOffset;
		removed->size = aux.size;
		V_pushback(removed, builder);
		I_pop(&aux, index);
	}
	free(fields);
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);

	if(nRemoved > 0) {
		// Ordenar crescentemente por level e depois por tamanho
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpbr);
		builder = V_new(sizeof(REMOVED_LIST_NODE));
		offset = auxH.first;
		while(offset != L_NIL) {
			fseek(stream, offset, SEEK_SET);
			R_read(&aux, stream);
			removedAux.nextByteOffset = aux.next;
			removedAux.byteOffset = offset;
			removedAux.size = aux.size;
			removedAux.level = 0;
			V_pushback(&removedAux, builder);
			offset = aux.next;
		}
		removed = (REMOVED_LIST_NODE *) realloc(removed, sizeof(REMOVED_LIST_NODE) * (nRemoved + V_size(builder)));
		V_copy(&removed[nRemoved], builder);
		nRemoved += V_size(builder);
		V_destroy(builder);
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmplr);
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmps);
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	I_rewrite(index);
	I_destroy(index);

	free(removed);
	fclose(stream);
	return 1;
}

int f13_addRegsKey(char *binPath, char *indPath, int n, FILE *input) {
	char found, field[5000];
	long offset;
	int i, j, nRemoved;
	REMOVED_LIST_NODE *removed;
	VECTOR_T *builder;
	REG_HEADER auxH;
	FILE *stream;
	IND_T *index;
	REG_DATA aux, *toBeAdded;
	double k;

	stream = fopen(binPath, "r+b");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	auxH.status = STATUS_INCONSISTENTE;
	R_writeH(&auxH, stream);

	index = I_init(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	// Ler lista de removidos
	builder = V_new(sizeof(REMOVED_LIST_NODE));
	removed = (REMOVED_LIST_NODE *) malloc(sizeof(REMOVED_LIST_NODE));
	offset = auxH.first;
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		removed->nextByteOffset = aux.next;
		removed->byteOffset = offset;
		removed->size = aux.size;
		V_pushback(removed, builder);
		offset = aux.next;
	}
	free(removed);
	nRemoved = V_build(&removed, builder);
	V_destroy(builder);

	// Adiciona um por um: tenta na lista de removidos da RAM. Caso não dê: adiciona em lista na RAM pra adicionar ao fim.
	builder = V_new(sizeof(REG_DATA));
	aux.removed = R_NAO_REMOVIDO;
	for(i = 0; i < n; i++) {
		scanf("%d", &aux.idServidor);
		scan_quote_string(field);
		if(field[0] == '\0') {
			aux.salarioServidor = -1;
		} else {
			aux.salarioServidor = atof(field);
		}
		scan_quote_string(field);
		strncpy(aux.telefoneServidor, field, 14);
		scan_quote_string(aux.nomeServidor);
		scan_quote_string(aux.cargoServidor);
		aux.size = R_calculateSize(&aux);
		for(j = 0; j < nRemoved; j++) {
			if(removed[j].size >= aux.size) {
				break;
			}
		}
		if(j >= nRemoved) { // não cabe em nenhum
			V_pushback(&aux, builder);
			continue;
		}
		aux.size = removed[j].size;
		aux.byteOffset = removed[j].byteOffset;
		if(aux.nomeServidor[0] != '\0') {
			I_push(&aux, index);
		}
		R_rewriteBack(&aux, stream);
		memmove(&removed[j], &removed[j + 1], sizeof(REMOVED_LIST_NODE) * (nRemoved - j - 1));
		nRemoved--;
	}
	n = V_build(&toBeAdded, builder);
	V_destroy(builder);

	// Encontrar fim do arquivo.
	fseek(stream, 0, SEEK_END);
	offset = ftell(stream);
	if(offset % 32000 == 0) {
		offset = -1;
	} else {
		while(R_read(&aux, stream) == 1) {
			offset = aux.size;
		}
	}

	// Adicionar os que faltam
	for(i = 0; i < n; i++) {
		R_write(&toBeAdded[i], stream, &offset);
		if(toBeAdded[i].nomeServidor[0] != '\0') {
			I_push(&toBeAdded[i], index);
		}
	}

	// Atualizar lista
	if(nRemoved > 0) {
		// Não ordena: pra manter ordem original.
		// Ajustar encadeamento da lista
		for(i = 1; i < nRemoved; i++) {
			removed[i - 1].nextByteOffset = removed[i].byteOffset;
		}
		removed[nRemoved - 1].nextByteOffset = L_NIL;
		auxH.first = removed[0].byteOffset;
		// Ordenar crescentemente por byteOffset: melhora desempenho na escrita final dependendo do S.O.
		MS_sort(removed, nRemoved, sizeof(REMOVED_LIST_NODE), (int (*)(const void *, const void *)) node_cmpb);
	} else {
		auxH.first = L_NIL;
	}

	for(i = 0; i < nRemoved; i++) {
		aux.removed = R_REMOVIDO;
		aux.size = removed[i].size;
		aux.next = removed[i].nextByteOffset;
		aux.byteOffset = removed[i].byteOffset;
		R_rewriteBack(&aux, stream);
	}

	auxH.status = STATUS_CONSISTENTE;
	R_writeH(&auxH, stream);

	I_rewrite(index);
	I_destroy(index);

	fclose(stream);
	return 1;
}

int f14_searchKeyStats(char *binPath, char *indPath, char *value) {
	long lastPage, pageCount;
	REG_HEADER auxH;
	IND_DATA *items;
	FILE *stream;
	REG_DATA aux;
	IND_T *index;
	char found;
	int i, n;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	index = I_init(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	
	items = I_iterate(&n, value, index);
	/*if(n <= 0) {
		printf("Registro inexistente.\n");
		return 0;
	}*/

	printf("*** Realizando a busca sem o auxílio de índice\n");	
	found = 0;
	// Rodar sem o índice.
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO || strcmp(aux.nomeServidor, value) != 0) {
			continue;
		}
		found = 1;
		printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
		if(aux.salarioServidor >= 0) {
			printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
		}
		if(strncmp(aux.telefoneServidor, "", 14) != 0) {
			printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
		}
		if(strcmp(aux.cargoServidor, "") != 0) {
			printf("%s\n", aux.cargoServidor);
		} else {
			printf("valor nao declarado\n");
		}
		printf("\n");
	}
	if(!found) {
		printf("Registro inexistente.\n");
	}
	printf("Número de páginas de disco acessadas: %ld\n", B_count(stream));

	printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
	found = 0;
	for(i = 0; i < n; i++) {
		fseek(stream, items[i].byteOffset, SEEK_SET);
		if(R_read(&aux, stream) != 1 || aux.removed == R_REMOVIDO) {
			printf("Falha no processamento do arquivo.\n");
			return 0;
		}
		if(strcmp(aux.nomeServidor, value) != 0) {
			continue;
		}
		found = 1;
		printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
		if(aux.salarioServidor >= 0) {
			printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
		}
		if(strncmp(aux.telefoneServidor, "", 14) != 0) {
			printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
		}
		if(strcmp(aux.cargoServidor, "") != 0) {
			printf("%s\n", aux.cargoServidor);
		} else {
			printf("valor nao declarado\n");
		}
		printf("\n");
	}

	pageCount = 1;
	lastPage = -1;
	for(i = 0; i < n; i++) {
		if(items[i].byteOffset/PAGE_SIZE != lastPage) {
			pageCount++;
		}
		lastPage = items[i].byteOffset / PAGE_SIZE;
	}

	if(!found) {
		printf("Registro inexistente.\n");
	}

	printf("Número de páginas de disco para carregar o arquivo de índice: %ld\n", I_count(index));
	printf("Número de páginas de disco para acessar o arquivo de dados: %ld\n", pageCount);

	fseek(stream, 0, SEEK_END);
	pageCount = B_count(stream) - pageCount;
	printf("\nA diferença no número de páginas de disco acessadas: %ld\n", pageCount);

	I_destroy(index);
	fclose(stream);
	return 1;
}

int f15_indexB(char *binPath, char *indPath) {
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	BTR_T *index;
	int i;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	index = BT_new(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	//char teste[100];
	//fgets(teste, 100, stdin);
	while(R_read(&aux, stream) == 1) {
		if(aux.removed == R_REMOVIDO) {
			continue;
		}
		//printf("pressione [ENTER] para inserir [%d = 0x%x]. ", aux.idServidor, (unsigned int) aux.idServidor);
		//fgets(teste, 100, stdin);
		if(aux.idServidor >= 0) {
			BT_push(&aux, index);
		}
		//BT_flush(index);
	}

	fclose(stream);

	if(BT_close(index) != 1) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	return 1;
}

int f16_searchKeyB(char *binPath, char *indPath, char *value) {
	long levelCount, offset;
	REG_HEADER auxH;
	FILE *stream;
	REG_DATA aux;
	BTR_T *index;
	char found;
	int i;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	index = BT_init(indPath);
	if(index == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	offset = BT_search(atoi(value), index);

	found = 0;
	fseek(stream, offset, SEEK_SET);
	if(R_read(&aux, stream) != 1 || aux.removed == R_REMOVIDO) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	if(aux.idServidor == atoi(value)) {
		found = 1;
		printf("%s: %d\n%s: ", auxH.desCampos[0], aux.idServidor, auxH.desCampos[1]);
		if(aux.salarioServidor >= 0) {
			printf("%.2lf\n%s: ", aux.salarioServidor, auxH.desCampos[2]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[2]);
		}
		if(strncmp(aux.telefoneServidor, "", 14) != 0) {
			printf("%.14s\n%s: ", aux.telefoneServidor, auxH.desCampos[3]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[3]);
		}
		if(strcmp(aux.nomeServidor, "") != 0) {
			printf("%s\n%s: ", aux.nomeServidor, auxH.desCampos[4]);
		} else {
			printf("valor nao declarado\n%s: ", auxH.desCampos[4]);
		}
		if(strcmp(aux.cargoServidor, "") != 0) {
			printf("%s\n", aux.cargoServidor);
		} else {
			printf("valor nao declarado\n");
		}
		printf("\n");
	}

	levelCount = BT_count(index);
	BT_close(index);

	if(!found) {
		printf("Registro inexistente.\n");
	} else {
		printf("Número de níveis do índice árvore-B percorridos: %ld\n", levelCount);
	}

	fclose(stream);
	return 1;
}



int fx_listRemoved(char *binPath) {
	REG_HEADER auxH;
	REG_DATA aux;
	FILE *stream;
	long offset;

	stream = fopen(binPath, "rb");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_readH(&auxH, stream);
	if(auxH.status != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	offset = auxH.first;
	printf("Começa em 0x%016lX\n", offset);
	while(offset != L_NIL) {
		fseek(stream, offset, SEEK_SET);
		R_read(&aux, stream);
		printf("[0x%016lX] tamanho = %d próximo = 0x%016lX\n", offset, aux.size, aux.next);
		offset = aux.next;
	}

	fclose(stream);
	return 1;
}

int fx_invalidate(char *binPath) {
	FILE *stream;
	char R;

	stream = fopen(binPath, "rb+");
	if(!stream) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R = fgetc(stream);
	if(R != STATUS_CONSISTENTE) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R = STATUS_INCONSISTENTE;
	if(fseek(stream, 0, SEEK_SET) != 0 || fwrite(&R, sizeof(char), 1, stream) != 1) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	printf("Arquivo invalidado.\n");

	fclose(stream);
	return 1;
}
