#include <stdlib.h>
#include <string.h>



/*
 *	~         Pilha         ~
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



struct __stack_node_t {
	void *value;
	struct __stack_node_t *below;
};

struct __stack_t {
	struct __stack_node_t *topper, *bottom;
	unsigned long nitems;
	size_t memsize;
};



struct __stack_t *S_New(size_t memsize){
	struct __stack_t *Aux;
	if(memsize < 1)	return NULL;
	Aux = (struct __stack_t *) malloc(sizeof(struct __stack_t));
	Aux->topper = Aux->bottom = NULL;
	Aux->memsize = memsize;
	Aux->nitems = 0;
	return Aux;
}

long S_Size(struct __stack_t *S){
	if(S == NULL)	return -1;
	return S->nitems;
}

int S_Push(void *X, struct __stack_t *S){
	struct __stack_node_t *Aux;
	if(X == NULL || S == NULL)	return 0;
	Aux = (struct __stack_node_t *) malloc(sizeof(struct __stack_node_t));
	Aux->value = (void *) malloc(S->memsize);
	Aux->below = S->topper;
	memcpy(Aux->value, X, S->memsize);
	S->topper = Aux;
	if(S->nitems < 1)
		S->bottom = Aux;
	S->nitems++;
	return 1;
}

int S_Top(void *X, struct __stack_t *S){
	if(X == NULL || S == NULL)	return 0;
	if(S->nitems < 1)	return 0;
	memcpy(X, S->topper->value, S->memsize);
	return 1;
}

int S_Bottom(void *X, struct __stack_t *S){
	if(X == NULL || S == NULL)	return 0;
	if(S->nitems < 1)	return 0;
	memcpy(X, S->bottom->value, S->memsize);
	return 1;
}

int S_Pop(void *X, struct __stack_t *S){
	struct __stack_node_t *Aux;
	if(S == NULL)	return 0;
	if(S->nitems < 1)	return 0;
	if(X != NULL)
		memcpy(X, S->topper->value, S->memsize);
	Aux = S->topper;
	S->topper = S->topper->below;
	S->nitems--;
	if(S->nitems < 1)
		S->bottom = NULL;
	free(Aux->value);
	free(Aux);
	return 1;
}

void S_Destroy(struct __stack_t *S){
	struct __stack_node_t *P, *Aux;
	if(S == NULL)	return;
	P = S->topper;
	while(P != NULL) {
		Aux = P;
		P = P->below;
		free(Aux->value);
		free(Aux);
	}
	free(S);
}


