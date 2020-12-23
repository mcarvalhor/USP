#include <stdlib.h>
#include <string.h>



/*
 *	~         Deque         ~
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



struct __deque_node_t {
	void *value;
	struct __deque_node_t *prev, *next;
};

struct __deque_t {
	struct __deque_node_t *first, *last;
	unsigned long nitems;
	size_t memsize;
};



struct __deque_t *DQ_New(size_t memsize){
	struct __deque_t *Aux;
	if(memsize < 1)	return NULL;
	Aux = (struct __deque_t *) malloc(sizeof(struct __deque_t));
	Aux->first = Aux->last = NULL;
	Aux->memsize = memsize;
	Aux->nitems = 0;
	return Aux;
}

long DQ_Size(struct __deque_t *DQ){
	if(DQ == NULL)	return -1;
	return DQ->nitems;
}

int DQ_PushFront(void *X, struct __deque_t *DQ){
	struct __deque_node_t *Aux;
	if(DQ == NULL || X == NULL)	return 0;
	Aux = (struct __deque_node_t *) malloc(sizeof(struct __deque_node_t));
	Aux->value = (void *) malloc(DQ->memsize);
	Aux->prev = NULL;
	Aux->next = DQ->first;
	memcpy(Aux->value, X, DQ->memsize);
	if(DQ->nitems > 0)
		DQ->first->prev = Aux;
	else
		DQ->last = Aux;
	DQ->first = Aux;
	DQ->nitems++;
	return 1;
}

int DQ_PushBack(void *X, struct __deque_t *DQ){
	struct __deque_node_t *Aux;
	if(DQ == NULL || X == NULL)	return 0;
	Aux = (struct __deque_node_t *) malloc(sizeof(struct __deque_node_t));
	Aux->value = (void *) malloc(DQ->memsize);
	Aux->prev = DQ->last;
	Aux->next = NULL;
	memcpy(Aux->value, X, DQ->memsize);
	if(DQ->nitems > 0)
		DQ->last->next = Aux;
	else
		DQ->first = Aux;
	DQ->last = Aux;
	DQ->nitems++;
	return 1;
}

int DQ_Front(void *X, struct __deque_t *DQ){
	if(DQ == NULL || X == NULL)	return 0;
	if(DQ->nitems < 1)	return 0;
	memcpy(X, DQ->first->value, DQ->memsize);
	return 1;
}

int DQ_Back(void *X, struct __deque_t *DQ){
	if(DQ == NULL || X == NULL)	return 0;
	if(DQ->nitems < 1)	return 0;
	memcpy(X, DQ->last->value, DQ->memsize);
	return 1;
}

int DQ_ShiftFront(void *X, struct __deque_t *DQ){
	struct __deque_node_t *Aux;
	if(DQ == NULL)	return 0;
	if(DQ->nitems < 1)	return 0;
	if(X != NULL)
		memcpy(X, DQ->first->value, DQ->memsize);
	Aux = DQ->first;
	DQ->first = DQ->first->next;
	DQ->nitems--;
	if(DQ->nitems < 1)
		DQ->last = NULL;
	free(Aux->value);
	free(Aux);
	return 1;
}

int DQ_ShiftBack(void *X, struct __deque_t *DQ){
	struct __deque_node_t *Aux;
	if(DQ == NULL)	return 0;
	if(DQ->nitems < 1)	return 0;
	if(X != NULL)
		memcpy(X, DQ->last->value, DQ->memsize);
	Aux = DQ->last;
	DQ->last = DQ->last->prev;
	DQ->nitems--;
	if(DQ->nitems < 1)
		DQ->first = NULL;
	free(Aux->value);
	free(Aux);
	return 1;
}

void DQ_Destroy(struct __deque_t *DQ){
	struct __deque_node_t *P, *Aux;
	if(DQ == NULL)	return;
	P = DQ->first;
	while(P != NULL) {
		Aux = P;
		P = P->next;
		free(Aux->value);
		free(Aux);
	}
	free(DQ);
}


