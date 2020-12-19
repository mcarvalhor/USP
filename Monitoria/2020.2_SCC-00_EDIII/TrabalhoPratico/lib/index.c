#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "consts.h"
#include "register.h"
#include "index.h"

IND_T *I_init(char *path) {
	IND_T *aux;

	aux = (IND_T *) malloc(sizeof(IND_T));
	aux->status = STATUS_CONSISTENTE;
	aux->nItems = 0;
	aux->data = NULL;
	aux->path[0] = '\0';

	if(path != NULL) {
		if(I_read(aux, path) != 1) {
			I_destroy(aux);
			return NULL;
		}
	}

	return aux;
}

int I_read(IND_T *index, char *path) {
	FILE *stream;
	char aux[8];
	int i;

	stream = fopen(path, "rb");
	if(stream == NULL) {
		return 0;
	}

	if(fread(aux, sizeof(char), 8, stream) != 8) {
		fclose(stream);
		return 0;
	}

	if(aux[0] != STATUS_CONSISTENTE) {
		fclose(stream);
		return 0;
	}

	fseek(stream, 0, SEEK_END);
	index->status = STATUS_CONSISTENTE;
	index->nItems = (ftell(stream) - 8) / 8;
	fseek(stream, 8, SEEK_SET);

	if(index->nItems < 0) {
		fclose(stream);
		return 0;
	}

	strcpy(index->path, path);
	if(index->data != NULL) {
		free(index->data);
	}

	index->data = (IND_DATA *) malloc(sizeof(IND_DATA) * index->nItems);

	for(i = 0; i < index->nItems; i++) {
		if(fread(aux, sizeof(char), 8, stream) != 8) {
			fclose(stream);
			return 0;
		}
		memcpy(&index->data[i].key, aux, sizeof(int));
		memcpy(&index->data[i].RRN, &aux[4], sizeof(int));
	}

	fclose(stream);
	return 1;
}

int I_rewrite(IND_T *index) {
	FILE *stream;
	char aux[8];
	int i;

	if(index->path[0] == '\0') {
		return 0;
	}

	stream = fopen(index->path, "wb");
	if(stream == NULL) {
		return 0;
	}

	index->status = STATUS_INCONSISTENTE;
	memset(aux, LIXO, 8);
	memcpy(aux, &index->status, sizeof(char));
	if(fwrite(aux, sizeof(char), 8, stream) != 8) {
		fclose(stream);
		return 0;
	}

	memset(aux, LIXO, 8);
	for(i = 0; i < index->nItems; i++) {
		memset(aux, LIXO, 8);
		memcpy(aux, &index->data[i].key, sizeof(int));
		memcpy(&aux[4], &index->data[i].RRN, sizeof(int));
		if(fwrite(aux, sizeof(char), 8, stream) != 8) {
			fclose(stream);
			return 0;
		}
	}

	index->status = STATUS_CONSISTENTE;
	memset(aux, LIXO, 8);
	memcpy(aux, &index->status, sizeof(char));
	fseek(stream, 0, SEEK_SET);
	if(fwrite(aux, sizeof(char), 8, stream) != 8) {
		fclose(stream);
		return 0;
	}

	fclose(stream);
	return 1;
}

int I_write(IND_T *index, char *path) {
	strcpy(index->path, path);
	return I_rewrite(index);
}

int I_push(REG_DATA *reg, IND_T *index) {
	int i, aux;

	for(i = 0; i < index->nItems; i++) {
		if(reg->idPessoa < index->data[i].key) {
			break;
		}
	}

	index->data = (IND_DATA *) realloc(index->data, sizeof(IND_DATA) * (index->nItems + 1));
	memmove(&index->data[i + 1], &index->data[i], sizeof(IND_DATA) * (index->nItems - i));

	index->data[i].RRN = reg->RRN;
	index->data[i].key = reg->idPessoa;
	
	index->nItems++;
	return 1;
}

int I_pop(REG_DATA *reg, IND_T *index) {
	int i, aux;

	for(i = 0; i < index->nItems; i++) {
		if(reg->RRN == index->data[i].RRN) {
			break;
		}
	}

	if(i >= index->nItems) {
		return 0;
	}

	memmove(&index->data[i], &index->data[i + 1], sizeof(IND_DATA) * (index->nItems - i - 1));
	//index->data = (IND_DATA *) realloc(index->data, sizeof(IND_DATA) * (index->header.nItems - 1));

	index->nItems--;
	return 1;
}

int I_update(REG_DATA *reg, IND_T *index) {
	int i, aux;

	if(I_pop(reg, index) == 0) {
		return 0;
	}

	return I_push(reg, index);
}

IND_DATA *I_find(int key, IND_T *index) {
	int i, j, aux;

	for(i = 0; i < index->nItems; i++) {
		if(key == index->data[i].key) {
			return &index->data[i];
		}
	}

	return NULL;
}

void I_destroy(IND_T *index) {
	if(index == NULL) {
		return;
	}
	free(index->data);
	free(index);
}

