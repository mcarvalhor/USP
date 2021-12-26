#include <stdlib.h>
#include <string.h>

typedef struct __VECTOR_NODE_T {
	void *item;
	struct __VECTOR_NODE_T *next;
} VECTOR_NODE_T;


typedef struct __VECTOR_T {
	VECTOR_NODE_T *first;
	unsigned long n;
	size_t memsize;
} VECTOR_T;



VECTOR_T *V_new(size_t memsize) {
	VECTOR_T *aux;
	if(memsize < 1) {
		return NULL;
	}

	aux = (VECTOR_T *) malloc(sizeof(VECTOR_T));
	aux->memsize = memsize;
	aux->first = NULL;
	aux->n = 0;
	return aux;
}

int V_pushback(void *item, VECTOR_T *v) {
	VECTOR_NODE_T *aux;
	if(item == NULL || v == NULL) {
		return 0;
	}
	aux = (VECTOR_NODE_T *) malloc(sizeof(VECTOR_NODE_T));
	aux->item = (void *) malloc(v->memsize);
	aux->next = v->first;
	v->first = aux;
	v->n++;

	memcpy(aux->item, item, v->memsize);
	return 1;
}

unsigned long V_size(VECTOR_T *v) {
	return v->n;
}


unsigned long V_copy(void *dest, VECTOR_T *v) {
	VECTOR_NODE_T *n;
	void *p;
	if(v == NULL || v->n < 1) {
		return 0;
	}
	p = dest + (v->n - 1) * v->memsize;
	n = v->first;
	while(n != NULL) {
		memcpy(p, n->item, v->memsize);
		p -= v->memsize;
		n = n->next;
	}
	return v->n;
}

unsigned long V_build(void *dest, VECTOR_T *v) {
	VECTOR_NODE_T *n;
	void *aux, *p;
	if(v == NULL || v->n < 1) {
		aux = NULL;
		memcpy(dest, &aux, sizeof(void *));
		return 0;
	}
	aux = (void *) malloc(v->memsize * v->n);
	p = aux + (v->n - 1) * v->memsize;
	n = v->first;
	while(n != NULL) {
		memcpy(p, n->item, v->memsize);
		p -= v->memsize;
		n = n->next;
	}
	memcpy(dest, &aux, sizeof(void *));
	return v->n;
}

int V_clear(VECTOR_T *v) {
	VECTOR_NODE_T *n, *aux;
	if(v == NULL) {
		return 0;
	}
	n = v->first;
	while(n != NULL) {
		aux = n;
		n = n->next;
		free(aux->item);
		free(aux);
	}
	v->first = NULL;
	v->n = 0;
	return 1;
}

void V_destroy(VECTOR_T *v) {
	VECTOR_NODE_T *n, *aux;
	if(v == NULL) {
		return;
	}
	n = v->first;
	while(n != NULL) {
		aux = n;
		n = n->next;
		free(aux->item);
		free(aux);
	}
	free(v);
}

