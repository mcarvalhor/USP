#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <consts.h>



typedef struct __INDEX_NODE_T {
	char key[INDEX_KEY_MAX_LEN + 1];
	unsigned long value;
	unsigned long index;
	struct __INDEX_NODE_T *next;
} INDEX_NODE_T;

typedef struct __INDEX_T {
	INDEX_NODE_T **hashTable;
	INDEX_NODE_T **vector;
	unsigned long keyCount;
	char vectorReady; // bool
} INDEX_T;



int I_loadFrom(char *, INDEX_T *);
void I_destroy(INDEX_T *);



INDEX_T *I_new() {
	INDEX_T *aux;

	aux = (INDEX_T *) malloc(sizeof(INDEX_T));
	if(aux == NULL) {
		return NULL;
	}

	aux->hashTable = (INDEX_NODE_T **) malloc(sizeof(INDEX_NODE_T *) * INDEX_HASHTABLE_LEN);
	if(aux->hashTable == NULL) {
		free(aux);
		return NULL;
	}
	memset(aux->hashTable, 0 /* NULL */, sizeof(INDEX_NODE_T *) * INDEX_HASHTABLE_LEN);
	aux->keyCount = 0;
	aux->vector = NULL;
	aux->vectorReady = 0;
	return aux;
}

INDEX_T *I_newFrom(char *filename) {
	INDEX_T *aux;
	if(filename == NULL) {
		return NULL;
	}

	aux = I_new();
	if(aux == NULL) {
		return NULL;
	}

	if(I_loadFrom(filename, aux) != 1) {
		I_destroy(aux);
		return NULL;
	}
}

unsigned long I_hashKey(char *key, unsigned long *length) {
	unsigned long i, count;
	for(i = count = 0; key[i] != '\0'; i++) {
		count += key[i];
	}
	if(length != NULL) {
		*length = i;
	}
	count = count % INDEX_HASHTABLE_LEN;
	return count;
}

int I_insertCount(char *key, unsigned long count, INDEX_T *ind) {
	INDEX_NODE_T **nodeI, *aux;
	long len, pos;

	if(key == NULL || ind == NULL) {
		return 0;
	}

	pos = I_hashKey(key, &len);
	if(len > INDEX_KEY_MAX_LEN) {
		return 0;
	}

	nodeI = &ind->hashTable[pos];
	while(*nodeI != NULL) {
		if(strcmp((*nodeI)->key, key) == 0) {
			if((*nodeI)->value < 1) {
				ind->keyCount++;
			}
			(*nodeI)->value += count;
			ind->vectorReady = 0;
			return 1;
		}
		nodeI = &(*nodeI)->next;
	}

	aux = (INDEX_NODE_T *) malloc(sizeof(INDEX_NODE_T));
	if(aux == NULL) {
		return 0;
	}
	
	aux->value = count;
	aux->next = NULL;
	strcpy(aux->key, key);
	*nodeI = aux;
	ind->keyCount++;
	ind->vectorReady = 0;
	return 1;
}

int I_insert(char *key, INDEX_T *ind) {
	return I_insertCount(key, 1, ind);
}

int I_removeCount(char *key, unsigned long count, INDEX_T *ind) {
	INDEX_NODE_T **nodeI, *aux;
	long len, pos;

	if(key == NULL || ind == NULL) {
		return 0;
	}

	pos = I_hashKey(key, &len);
	if(len > INDEX_KEY_MAX_LEN) {
		return 0;
	}

	nodeI = &ind->hashTable[pos];
	while(*nodeI != NULL) {
		if(strcmp((*nodeI)->key, key) == 0) {
			if((*nodeI)->value < 1) {
				return 0;
			}
			if(count >= (*nodeI)->value) {
				(*nodeI)->value = 0;
			} else {
				(*nodeI)->value -= count;
			}
			ind->vectorReady = 0;
			if((*nodeI)->value < 1) {
				ind->keyCount--;
			}
			return 1;
		}
		nodeI = &(*nodeI)->next;
	}
	return 0;
}

int I_remove(char *key, INDEX_T *ind) {
	return I_removeCount(key, 1, ind);
}

int I_sortCmp(INDEX_NODE_T **a, INDEX_NODE_T **b) {
	return strcmp((*a)->key, (*b)->key);
}

int I_sort(INDEX_T *ind) {
	INDEX_NODE_T **nodeI;
	unsigned long i, j;

	if(ind == NULL) {
		return 0;
	}
	if(ind->vectorReady) {
		return 1;
	}
	if(ind->keyCount < 1) {
		ind->vector = NULL;
		return 1;
	}
	if(ind->vector != NULL) {
		free(ind->vector);
	}

	ind->vector = (INDEX_NODE_T **) malloc(sizeof(INDEX_NODE_T *) * ind->keyCount);
	if(ind->vector == NULL) {
		return 0;
	}
	
	for(i = j = 0; i < INDEX_HASHTABLE_LEN; i++) {
		nodeI = &ind->hashTable[i];
		while(*nodeI != NULL) {
			if((*nodeI)->value > 0) {
				ind->vector[j] = *nodeI;
				j++;
			}
			nodeI = &(*nodeI)->next;
		}
	}

	qsort(ind->vector, ind->keyCount, sizeof(INDEX_NODE_T *), (int (*)(const void *, const void *)) I_sortCmp);
	for(j = 0; j < ind->keyCount; j++) {
		ind->vector[j]->index = j;
	}

	ind->vectorReady = 1;
	return 1;
}

int I_fillRestOfKey(char *key, INDEX_T *ind) {
	INDEX_NODE_T *aux;
	long len, pos;

	if(key == NULL || ind == NULL || I_sort(ind) != 1) {
		return -1;
	}

	for(pos = 0; pos < INDEX_HASHTABLE_LEN; pos++) {
		aux = ind->hashTable[pos];
		while(aux != NULL) {
			if(strncmp(aux->key, key, strlen(key)) == 0) {
				strcpy(key, aux->key);
				return 1;
			}
			aux = aux->next;
		}
	}
	return 0;
}

long I_getIndexFor(char *key, INDEX_T *ind) {
	INDEX_NODE_T *aux;
	long len, pos;

	if(key == NULL || ind == NULL || I_sort(ind) != 1) {
		return -1;
	}

	pos = I_hashKey(key, &len);
	if(len > INDEX_KEY_MAX_LEN) {
		return -1;
	}

	aux = ind->hashTable[pos];
	while(aux != NULL) {
		if(strcmp(aux->key, key) == 0) {
			return aux->index;
		}
		aux = aux->next;
	}
	return -2;
}

char *I_getKeyFor(long index, INDEX_T *ind) {
	INDEX_NODE_T *aux;

	if(ind == NULL || I_sort(ind) != 1 || index < 0 || index >= ind->keyCount) {
		return NULL;
	}

	aux = ind->vector[index];
	return aux->key;
}

long I_keyCount(INDEX_T *ind) {
	if(ind == NULL) {
		return -1;
	}
	return ind->keyCount;
}

int I_loadFrom(char *filename, INDEX_T *ind) {
	char buff[INDEX_KEY_MAX_LEN + sizeof(int)], status, key[INDEX_KEY_MAX_LEN + 1];
	FILE *stream;
	int value;
	if(filename == NULL || ind == NULL || (stream = fopen(filename, "rb")) == NULL) {
		return 0;
	}
	if(fread(&status, sizeof(char), 1, stream) != 1) {
		fclose(stream);
		return 0;
	}
	if(status != R_TRUE) {
		fclose(stream);
		return 0;
	}
	while(fread(buff, INDEX_KEY_MAX_LEN + sizeof(int), 1, stream) == 1) {
		key[INDEX_KEY_MAX_LEN] = '\0';
		strncpy(key, buff, INDEX_KEY_MAX_LEN);
		memcpy(&value, &buff[INDEX_KEY_MAX_LEN], sizeof(int));
		I_insertCount(key, value, ind);
	}
	fclose(stream);
	return 1;
}

int I_saveTo(char *filename, INDEX_T *ind) {
	char buff[INDEX_KEY_MAX_LEN + sizeof(int)], status, saved;
	unsigned long i, aux;
	FILE *stream;
	int value;
	if(filename == NULL || ind == NULL || I_sort(ind) != 1 || (stream = fopen(filename, "wb")) == NULL) {
		return 0;
	}
	status = R_TRUE;
	if(fwrite(&status, sizeof(char), 1, stream) != 1) {
		fclose(stream);
		return 0;
	}
	saved = 1;
	for(i = 0; i < ind->keyCount; i++) {
		memset(buff, R_LIXO, sizeof(char) * (INDEX_KEY_MAX_LEN + sizeof(int)));
		strcpy(buff, ind->vector[i]->key);
		memcpy(&aux, &ind->vector[i]->value, sizeof(unsigned long));
		value = (int) aux;
		memcpy(&buff[INDEX_KEY_MAX_LEN], &value, sizeof(int));
		if(fwrite(buff, INDEX_KEY_MAX_LEN + sizeof(int), 1, stream) != 1) {
			saved = 0;
		}
	}
	fclose(stream);
	if(saved != 1) {
		saved = 0;
	}
	return saved;
}

void I_destroy(INDEX_T *ind) {
	INDEX_NODE_T *nodeI, *aux;
	unsigned long i, j;
	if(ind == NULL) {
		return;
	}
	if(ind->vector != NULL) {
		free(ind->vector);
	}
	for(i = 0; i < INDEX_HASHTABLE_LEN; i++) {
		nodeI = ind->hashTable[i];
		while(nodeI != NULL) {
			aux = nodeI;
			nodeI = nodeI->next;
			free(aux);
		}
	}
	free(ind);
}