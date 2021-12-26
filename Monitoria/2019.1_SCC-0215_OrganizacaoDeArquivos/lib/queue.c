#include <stdlib.h>
#include <stdio.h>


typedef struct _QUEUE_NODE_T {
	void *item;
	struct _LIST_NODE_T *next;
} QUEUE_NODE_T;

typedef struct _QUEUE_T {
	QUEUE_NODE_T *first, *last;
	unsigned long n;
	size_t memsize;
} QUEUE_T;


QUEUE_T *Q_new(size_t memsize) {
	QUEUE_T *aux;
	if(memsize < 1) {
		return NULL;
	}

	aux = (QUEUE_T *) malloc(sizeof(QUEUE_T));
	aux->first = aux->last = NULL;
	aux->memsize = memsize;
	aux->n = 0;

	return aux;
}

int Q_push(void *item, QUEUE_T *list) {
	if(list == NULL || item == NULL) {
		return -1;
	}

	if(list->last == NULL) {
		list->first = list->last = (QUEUE_NODE_T *) malloc(sizeof(QUEUE_NODE_T));
	} else {
		list->last->next = (QUEUE_NODE_T *) malloc(sizeof(QUEUE_NODE_T));
		list->last = list->last->next;
	}

	list->last->item = (void *) malloc(list->memsize);
	list->last->next = NULL;

	memcpy(list->last->item, item, list->memsize);
	list->n++;
	return 1;
}

int Q_shift(void *item, QUEUE_T *list) {
	QUEUE_NODE_T *aux;
	if(list == NULL) {
		return -1;
	}

	if(list->first == NULL) {
		return 0;
	}

	if(item != NULL) {
		memcpy(item, list->first->item, list->memsize);
	}
	aux = list->first;
	list->first = list->first->next;
	free(aux->item);
	free(aux);

	if(list->first == NULL) {
		list->last = NULL;
	}

	list->n--;
	return 1;
}

unsigned long Q_size(QUEUE_T *list) {
	if(list == NULL) {
		return 0;
	}
	return list->n;
}

int Q_clear(QUEUE_T *list) {
	QUEUE_NODE_T *n, *aux;
	if(list == NULL) {
		return -1;
	}

	n = list->first;
	while(n != NULL) {
		aux = n;
		n = n->next;
		free(aux->item);
		free(aux);
	}

	list->first = list->last = NULL;
	list->n = 0;
	return 1;
}

void Q_destroy(QUEUE_T *list) {
	QUEUE_NODE_T *n, *aux;
	if(list == NULL) {
		return;
	}

	n = list->first;
	while(n != NULL) {
		aux = n;
		n = n->next;
		free(aux->item);
		free(aux);
	}

	free(list);
}
