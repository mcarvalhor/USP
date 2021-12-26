#include <stdlib.h>
#include <string.h>



struct __stack_t {
	void *values;
	size_t memsize;
	unsigned long nitems;
};



struct __stack_t *S_New(size_t memsize){
	if(memsize<1)	return NULL;
	struct __stack_t *Aux=(struct __stack_t *)calloc(1,sizeof(struct __stack_t));
	Aux->memsize=memsize;
	return Aux;
}

int S_Push(void *Elem, struct __stack_t *S){
	if(Elem==NULL || S==NULL)	return 0;
	S->values=(void *)realloc(S->values,(++S->nitems)*S->memsize);
	memcpy(S->values+(S->nitems-1)*S->memsize,Elem,S->memsize);
	return 1;
}

int S_Get(void *Dest, struct __stack_t *S){
	if(Dest==NULL || S==NULL)	return 0;
	if(S->nitems<1)	return 0;
	memcpy(Dest,S->values+S->memsize*(S->nitems-1),S->memsize);
	return 1;
}

int S_Pop(void *Dest, struct __stack_t *S){
	if(S==NULL)	return 0;
	if(S->nitems<1)	return 0;
	memcpy(Dest,S->values+S->memsize*(--S->nitems),S->memsize);
	S->values=(void *)realloc(S->values,(S->nitems)*S->memsize);
	return 1;
}

void S_Destroy(struct __stack_t *S){
	if(S==NULL)	return;
	if(S->values!=NULL)	free(S->values);
	free(S);
}


