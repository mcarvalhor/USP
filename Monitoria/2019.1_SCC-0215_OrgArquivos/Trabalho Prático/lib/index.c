#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <consts.h>
#include <buffer.h>
#include <register.h>
#include <index.h>

IND_T *I_init(char *path) {
	IND_T *aux;

	aux = (IND_T *) malloc(sizeof(IND_T));
	aux->header.status = STATUS_CONSISTENTE;
	aux->header.nItems = 0;
	aux->accessCount = 0;
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
	char aux[PAGE_SIZE];
	FILE *stream;
	int i;

	stream = fopen(path, "rb");
	if(stream == NULL) {
		return 0;
	}

	if(fread(aux, sizeof(char), PAGE_SIZE, stream) != PAGE_SIZE) {
		fclose(stream);
		return 0;
	}

	if(aux[0] != STATUS_CONSISTENTE) {
		fclose(stream);
		return 0;
	}

	index->header.status = STATUS_CONSISTENTE;
	memcpy(&index->header.nItems, &aux[1], sizeof(int));

	strcpy(index->path, path);
	if(index->data != NULL) {
		free(index->data);
	}

	index->data = (IND_DATA *) malloc(sizeof(IND_DATA) * index->header.nItems);

	for(i = 0; i < index->header.nItems; i++) {
		index->data[i].RRN = i;
		if(fread(aux, sizeof(char), IND_DATA_SIZE, stream) != IND_DATA_SIZE) {
			fclose(stream);
			return 0;
		}
		strcpy(index->data[i].key, aux);
		memcpy(&index->data[i].byteOffset, &aux[120], sizeof(long));
	}

	index->accessCount += ftell(stream) / PAGE_SIZE;
	if(ftell(stream) % PAGE_SIZE != 0) {
		index->accessCount++;
	}

	fclose(stream);
	return 1;
}

int I_rewrite(IND_T *index) {
	char aux[PAGE_SIZE];
	FILE *stream;
	int i;

	if(index->path[0] == '\0') {
		return 0;
	}

	stream = fopen(index->path, "wb");
	if(stream == NULL) {
		return 0;
	}

	index->header.status = STATUS_INCONSISTENTE;
	memset(aux, LIXO, PAGE_SIZE);
	memcpy(aux, &index->header.status, sizeof(char));
	memcpy(&aux[1], &index->header.nItems, sizeof(int));
	if(fwrite(aux, sizeof(char), PAGE_SIZE, stream) != PAGE_SIZE) {
		fclose(stream);
		return 0;
	}

	memset(aux, LIXO, PAGE_SIZE);
	for(i = 0; i < index->header.nItems; i++) {
		memset(aux, LIXO, IND_DATA_SIZE);
		strcpy(aux, index->data[i].key);
		memcpy(&aux[120], &index->data[i].byteOffset, sizeof(long));
		if(fwrite(aux, sizeof(char), IND_DATA_SIZE, stream) != IND_DATA_SIZE) {
			fclose(stream);
			return 0;
		}
	}

	index->header.status = STATUS_CONSISTENTE;
	memset(aux, LIXO, PAGE_SIZE);
	memcpy(aux, &index->header.status, sizeof(char));
	memcpy(&aux[1], &index->header.nItems, sizeof(int));
	fseek(stream, 0, SEEK_SET);
	if(fwrite(aux, sizeof(char), PAGE_SIZE, stream) != PAGE_SIZE) {
		fclose(stream);
		return 0;
	}

	index->accessCount += ftell(stream) / PAGE_SIZE;
	if(ftell(stream) % PAGE_SIZE != 0) {
		index->accessCount++;
	}

	fclose(stream);
	return 1;
}

int I_write(IND_T *index, char *path) {
	strcpy(index->path, path);
	return I_rewrite(index);
}

long I_count(IND_T *index) {
	return index->accessCount;
}

void I_resetCount(IND_T *index) {
	index->accessCount = 0;
}

int I_push(REG_DATA *reg, IND_T *index) {
	int i, aux;

	for(i = 0; i < index->header.nItems; i++) {
		aux = strncmp(reg->nomeServidor, index->data[i].key, 120); 
		if(aux < 0 || (aux == 0 && reg->byteOffset < index->data[i].byteOffset)) {
			break;
		}
	}

	index->data = (IND_DATA *) realloc(index->data, sizeof(IND_DATA) * (index->header.nItems + 1));
	memmove(&index->data[i + 1], &index->data[i], sizeof(IND_DATA) * (index->header.nItems - i));

	index->data[i].byteOffset = reg->byteOffset;
	strcpy(index->data[i].key, reg->nomeServidor);
	
	index->header.nItems++;
	for(i = 0; i < index->header.nItems; i++) {
		index->data[i].RRN = i;
	}

	return 1;
}

int I_pop(REG_DATA *reg, IND_T *index) {
	int i, aux;

	for(i = 0; i < index->header.nItems; i++) {
		if(reg->byteOffset == index->data[i].byteOffset) {
			break;
		}
	}

	if(i >= index->header.nItems) {
		return 0;
	}

	memmove(&index->data[i], &index->data[i + 1], sizeof(IND_DATA) * (index->header.nItems - i - 1));
	//index->data = (IND_DATA *) realloc(index->data, sizeof(IND_DATA) * (index->header.nItems - 1));

	index->header.nItems--;
	for(i = 0; i < index->header.nItems; i++) {
		index->data[i].RRN = i;
	}

	return 1;
}

int I_update(REG_DATA *reg, IND_T *index) {
	int i, aux;

	for(i = 0; i < index->header.nItems; i++) {
		if(reg->byteOffset == index->data[i].byteOffset) {
			break;
		}
	}

	if(i >= index->header.nItems) {
		return 0;
	}

	memmove(&index->data[i], &index->data[i + 1], sizeof(IND_DATA) * (index->header.nItems - i - 1));

	index->header.nItems--;
	for(i = 0; i < index->header.nItems; i++) {
		aux = strcmp(reg->nomeServidor, index->data[i].key); 
		if(aux > 0 || (aux == 0 && reg->byteOffset > index->data[i].byteOffset)) {
			break;
		}
	}

	memmove(&index->data[i + 1], &index->data[i], sizeof(IND_DATA) * (index->header.nItems - i));

	index->data[i].byteOffset = reg->byteOffset;
	strcpy(index->data[i].key, reg->nomeServidor);
	
	index->header.nItems++;
	for(i = 0; i < index->header.nItems; i++) {
		index->data[i].RRN = i;
	}

	return 1;
}

IND_DATA *I_iterate(int *n, char *key, IND_T *index) {
	int i, j, aux;

	for(i = 0; i < index->header.nItems; i++) {
		aux = strcmp(key, index->data[i].key); 
		if(aux < 0) {
			*n = 0;
			return NULL;
		} else if(aux == 0) {
			break;
		}
	}

	if(i >= index->header.nItems) {
		*n = 0;
		return NULL;
	}

	for(j = i; j < index->header.nItems; j++) {
		if(strcmp(key, index->data[j].key) != 0) {
			break;
		}
	}

	*n = j - i;
	return &index->data[i];
}

void I_destroy(IND_T *index) {
	if(index == NULL) {
		return;
	}
	free(index->data);
	free(index);
}


