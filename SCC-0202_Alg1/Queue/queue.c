#include <stdlib.h>
#include <string.h>



/*
 *	~         Filas         ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



struct __queue_node_t {
	void *value;
	struct __queue_node_t *next;
};

struct __queue_t {
	struct __queue_node_t *first, *last;
	unsigned long nitems;
	size_t memsize;
};

struct __priority_queue_t {
	struct __queue_node_t *first, *last;
	int (*fcmp)(void *, void*);
	unsigned long nitems;
	size_t memsize;
};



struct __queue_t *Q_New(size_t memsize){
	struct __queue_t *Aux;
	if(memsize < 1)	return NULL;
	Aux = (struct __queue_t *) malloc(sizeof(struct __queue_t));
	Aux->first = Aux->last = NULL;
	Aux->memsize = memsize;
	Aux->nitems = 0;
	return Aux;
}

long Q_Size(struct __queue_t *Q){
	if(Q == NULL)	return -1;
	return Q->nitems;
}

int Q_Push(void *X, struct __queue_t *Q){
	struct __queue_node_t *Aux;
	if(Q == NULL || X == NULL)	return 0;
	Aux = (struct __queue_node_t *) malloc(sizeof(struct __queue_node_t));
	Aux->value = (void *) malloc(Q->memsize);
	Aux->next = NULL;
	memcpy(Aux->value, X, Q->memsize);
	if(Q->nitems > 0)
		Q->last->next = Aux;
	else
		Q->first = Aux;
	Q->last = Aux;
	Q->nitems++;
	return 1;
}

int Q_Front(void *X, struct __queue_t *Q){
	if(Q == NULL || X == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	memcpy(X, Q->first->value, Q->memsize);
	return 1;
}

int Q_Back(void *X, struct __queue_t *Q){
	if(Q == NULL || X == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	memcpy(X, Q->last->value, Q->memsize);
	return 1;
}

int Q_Shift(void *X, struct __queue_t *Q){
	struct __queue_node_t *Aux;
	if(Q == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	if(X != NULL)
		memcpy(X, Q->first->value, Q->memsize);
	Aux = Q->first;
	Q->first = Q->first->next;
	Q->nitems--;
	if(Q->nitems < 1)
		Q->last = NULL;
	free(Aux->value);
	free(Aux);
	return 1;
}

void Q_Destroy(struct __queue_t *Q){
	struct __queue_node_t *P, *Aux;
	if(Q == NULL)	return;
	P = Q->first;
	while(P != NULL) {
		Aux = P;
		P = P->next;
		free(Aux->value);
		free(Aux);
	}
	free(Q);
}



struct __priority_queue_t *PQ_New(size_t memsize, int (*fcmp)(void *, void *)){
	struct __priority_queue_t *Aux;
	if(memsize < 1 || fcmp == NULL)	return NULL;
	Aux = (struct __priority_queue_t *) malloc(sizeof(struct __priority_queue_t));
	Aux->first = Aux->last = NULL;
	Aux->memsize = memsize;
	Aux->fcmp = fcmp;
	Aux->nitems = 0;
	return Aux;
}

long PQ_Size(struct __priority_queue_t *Q){
	if(Q == NULL)	return -1;
	return Q->nitems;
}

int PQ_Push(void *X, struct __priority_queue_t *Q){
	struct __queue_node_t *Aux, *P, **Last;
	if(Q == NULL || X == NULL)	return 0;
	Aux = (struct __queue_node_t *) malloc(sizeof(struct __queue_node_t));
	Aux->value = (void *) malloc(Q->memsize);
	memcpy(Aux->value, X, Q->memsize);
	if(Q->nitems < 1) {
		Aux->next = NULL;
		Q->first = Q->last = Aux;
	} else if(Q->fcmp(Aux->value, Q->last->value) <= 0){
		Aux->next = NULL;
		Q->last = Q->last->next = Aux;
	} else {
		P = Q->first;
		Last = &Q->first;
		while(P != NULL) {
			if(Q->fcmp(Aux->value, P->value) > 0)
				break;
			Last = &P->next;
			P = P->next;
		}
		Aux->next = P;
		*Last = Aux;
	}
	Q->nitems++;
	return 1;
}

int PQ_Front(void *X, struct __priority_queue_t *Q){
	if(Q == NULL || X == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	memcpy(X, Q->first->value, Q->memsize);
	return 1;
}

int PQ_Back(void *X, struct __priority_queue_t *Q){
	if(Q == NULL || X == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	memcpy(X, Q->last->value, Q->memsize);
	return 1;
}

int PQ_Shift(void *X, struct __priority_queue_t *Q){
	struct __queue_node_t *Aux;
	if(Q == NULL)	return 0;
	if(Q->nitems < 1)	return 0;
	if(X != NULL)
		memcpy(X, Q->first->value, Q->memsize);
	Aux = Q->first;
	Q->first = Q->first->next;
	Q->nitems--;
	if(Q->nitems < 1)
		Q->last = NULL;
	free(Aux->value);
	free(Aux);
	return 1;
}

void PQ_Destroy(struct __priority_queue_t *Q){
	struct __queue_node_t *P, *Aux;
	if(Q == NULL)	return;
	P = Q->first;
	while(P != NULL) {
		Aux = P;
		P = P->next;
		free(Aux->value);
		free(Aux);
	}
	free(Q);
}


