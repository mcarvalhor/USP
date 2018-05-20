#include <stdlib.h>
#include <string.h>

void gswap(void *A, void *B, size_t memsize){
	void *Aux;
	if(A==NULL || B==NULL || A == B)	return;
	Aux = (void *) malloc(memsize);
	memcpy(Aux, B, memsize);
	memcpy(B, A, memsize);
	memcpy(A, Aux, memsize);
	free(Aux);
}


