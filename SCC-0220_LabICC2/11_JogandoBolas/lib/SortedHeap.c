#include <stdlib.h>
#include <math.h>
#include <assert.h>



/*
 *	~      SortedHeap       ~
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



enum __bool_t { false = 0, true };

struct __no_t {
	int Value;
	enum __bool_t Active;
};

struct __sortedheap_t {
	int N;
	struct __no_t *Root;
};



struct __sortedheap_t *New_SortedHeap(int level){
	if(level < 1)	return NULL;
	struct __sortedheap_t *Aux;
	int i,n;
	n=pow(2,level)-1;
	Aux=malloc(sizeof(struct __sortedheap_t));
	Aux->Root=calloc(n,sizeof(struct __no_t));
	Aux->N=n;
	for(i=0;i<n;i++)	Aux->Root[i].Value=i+1;
	return Aux;
}

int Traverse_ByStatus(struct __sortedheap_t *SH){
	assert(SH != NULL);
	int aux=SH->Root[0].Value, pos=0;
	while(pos < SH->N){
		aux=SH->Root[pos].Value;
		if(SH->Root[pos].Active==true){
			SH->Root[pos].Active=false;
			pos=pos*2+2;
		}else{
			SH->Root[pos].Active=true;
			pos=pos*2+1;
		}
	}
	return aux;
}

void Destroy_SortedHeap(struct __sortedheap_t *SH){
	if(SH==NULL)	return;
	free(SH->Root);
	free(SH);
}


