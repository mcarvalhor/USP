#include <stdlib.h>
#include <string.h>

struct __heap_t {
	void *values;
	size_t memsize;
	unsigned long nitems;
	int (*fcmp)(void *, void *);
};

struct __heap_t *H_New(size_t memsize, int (*fcmp)(void *, void *)){
	if(memsize<1 ||  fcmp==NULL)	return NULL;
	struct __heap_t *Aux=(struct __heap_t *)calloc(1,sizeof(struct __heap_t));
	Aux->memsize=memsize;
	Aux->fcmp=fcmp;
	return Aux;
}

int H_Add(void *Elem, struct __heap_t *H){
	if(Elem==NULL || H==NULL)	return 0;
	int i;
	for(i=0;i<H->nitems;i++)	if(H->fcmp(Elem,H->values+i*H->memsize)<=0)	break;
	H->values=(void *)realloc(H->values,(++H->nitems)*H->memsize);
	if(i<H->nitems-1)	memmove(H->values+(i+1)*H->memsize, H->values+i*H->memsize, H->memsize*(H->nitems-i-1));
	memcpy(H->values+i*H->memsize,Elem,H->memsize);
	return 1;
}

int H_Get(void *Dest, struct __heap_t *H){
	if(Dest==NULL || H==NULL)	return 0;
	if(H->nitems<1)	return 0;
	memcpy(Dest,H->values,H->memsize);
	return 1;
}

int H_Shift(void *Dest, struct __heap_t *H){
	if(Dest==NULL || H==NULL)	return 0;
	if(H->nitems<1)	return 0;
	memcpy(Dest,H->values,H->memsize);
	if(H->nitems>1)	memmove(H->values,H->values+H->memsize,H->memsize*(H->nitems-1));
	H->values=(void *)realloc(H->values,(--H->nitems)*H->memsize);
	return 1;
}

void H_Destroy(struct __heap_t *H){
	if(H==NULL)	return;
	if(H->values!=NULL)	free(H->values);
	free(H);
}

