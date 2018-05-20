#include <stdlib.h>
#include <string.h>

struct __queue_t {
	void *values;
	size_t memsize;
	unsigned long nitems;
};

struct __queue_t *Q_New(size_t memsize){
	if(memsize<1)	return NULL;
	struct __queue_t *Aux=(struct __queue_t *)calloc(1,sizeof(struct __queue_t));
	Aux->memsize=memsize;
	return Aux;
}

int Q_Size(struct __queue_t *Q){
	if(Q==NULL)	return -1;
	return Q->nitems;
}

char Q_Add(void *Elem, struct __queue_t *Q){
	if(Elem==NULL || Q==NULL)	return 0;
	Q->values=(void *)realloc(Q->values,(++Q->nitems)*Q->memsize);
	memcpy(Q->values+(Q->nitems-1)*Q->memsize,Elem,Q->memsize);
	return 1;
}

int Q_Get(void *Dest, struct __queue_t *Q){
	if(Dest==NULL || Q==NULL)	return 0;
	if(Q->nitems<1)	return 0;
	memcpy(Dest,Q->values,Q->memsize);
	return 1;
}

int Q_Shift(void *Dest, struct __queue_t *Q){
	if(Dest==NULL || Q==NULL)	return 0;
	if(Q->nitems<1)	return 0;
	memcpy(Dest,Q->values,Q->memsize);
	if(Q->nitems>1)	memmove(Q->values,Q->values+Q->memsize,Q->memsize*(Q->nitems-1));
	Q->values=(void *)realloc(Q->values,(--Q->nitems)*Q->memsize);
	return 1;
}

void Q_Destroy(struct __queue_t *Q){
	if(Q==NULL)	return;
	if(Q->values!=NULL)	free(Q->values);
	free(Q);
}

