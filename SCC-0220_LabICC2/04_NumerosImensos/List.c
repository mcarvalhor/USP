#include <stdlib.h>
#include <assert.h>
#include "List.h"


/*
 *      == Lista Enumerada ==
 *
 *      Matheus Carvalho Raimundo
 *      mcarvalhor@usp.br
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/


struct __ListItem_t{
	LIST_EL X;
	struct __ListItem_t *N;
};

struct __List_t{
	int size;
	struct __ListItem_t *start, *end;
};


struct __List_t *L_New(){
	struct __List_t *Aux=(struct __List_t *)malloc(sizeof(struct __List_t)*1);
	Aux->size=0;
	Aux->start=NULL;
	Aux->end=NULL;
	return Aux;
}

int L_Size(struct __List_t *L){
	if(L==NULL)	return -1;
	return L->size;
}

char L_Add(LIST_EL X,struct __List_t *L){
	if(L==NULL)	return -1;
	if(L->size==0){
		L->end=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
		L->end->X=X;
		L->end->N=NULL;
		L->size++;
		L->start=L->end;
		return 2;
	}
	struct __ListItem_t *Aux=L->end;
	Aux->N=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	Aux->N->X=X;
	Aux->N->N=NULL;
	L->end=Aux->N;
	L->size++;
	return 1;
}

char L_AddAt(LIST_EL X,int i,struct __List_t *L){
	if(L==NULL)	return -1;
	if(i<0 || i>L->size)	return 0;
	struct __ListItem_t *Aux=L->start;
	struct __ListItem_t *NewAux=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	if(L->size==0){
		NewAux->N=NULL;
		NewAux->X=X;
		L->start=L->end=NewAux;
	}else if(i==L->size){
		NewAux->N=NULL;
		NewAux->X=X;
		L->end->N=NewAux;
		L->end=NewAux;
	}else if(i==0){
		NewAux->N=L->start;
		NewAux->X=X;
		L->start=NewAux;
	}else if(i==L->size-1){
		NewAux->N=L->end->N;
		NewAux->X=X;
		L->end->N=NewAux;
		L->end=NewAux;
	}else{
		int p=0;
		while(p<i-1){
			Aux=Aux->N;
			p++;
		}
		NewAux->N=Aux->N->N;
		NewAux->X=X;
		Aux->N=NewAux;
	}
	L->size++;
	return 1;
}

char L_Remove(struct __List_t *L){
	if(L==NULL)	return -1;
	if(L->size<=0)	return 0;
	struct __ListItem_t *Aux=L->start;
	while(Aux->N!=L->end && Aux->N!=NULL){
		Aux=Aux->N;
	}
	Aux->N=NULL;
	free(L->end);
	L->end=Aux;
	L->size--;
	if(L->size<=0){
		L->end=L->start=NULL;
	}
	return 1;
}

char L_RemoveAt(int i,struct __List_t *L){
	if(L==NULL)	return -1;
	if(i<0 || i>=L->size)	return 0;
	struct __ListItem_t *Aux=L->start;
	struct __ListItem_t *R;
	if(L->size==1){
		free(Aux);
		L->start=NULL;
		L->end=NULL;
	}else if(i==0){
		R=Aux->N;
		free(Aux);
		L->start=R;
	}else{
		int p;
		for(p=0;p<i-1;p++)
			Aux=Aux->N;
		R=Aux->N;
		Aux->N=Aux->N->N;
		if(i==L->size-1){
			L->end=Aux;
		}
		free(R);
	}
	L->size++;
	return 1;
}

LIST_EL L_Get(struct __List_t *L){
	if(L==NULL)	assert(0);
	if(L->size<=0)	assert(0);
	return L->end->X;
}

LIST_EL L_GetAt(int i,struct __List_t *L){
	if(L==NULL)	assert(0);
	if(i<0 || i>=L->size)	assert(0);
	struct __ListItem_t *Aux;
	if(i==L->size-1){
		Aux=L->end;
		return Aux->X;
	}
	int p;
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	return Aux->X;
}

char L_Set(LIST_EL X,struct __List_t *L){
	if(L==NULL)	return -1;
	if(L->size<=0)	return 0;
	L->end->X=X;
	return 1;
}

char L_SetAt(LIST_EL X,int i,struct __List_t *L){
	if(L==NULL)	return -1;
	if(i<0 || i>=L->size)	return 0;
	struct __ListItem_t *Aux=L->start;
	int p;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	Aux->X=X;
	return 1;
}

char L_Destroy(struct __List_t *L){
	if(L==NULL)	return 0;
	if(L->size>0){
		struct __ListItem_t *Aux=L->start;
		struct __ListItem_t *R;
		while(Aux!=NULL){
			R=Aux->N;
			free(Aux);
			Aux=R;
		}
	}
	free(L);
	return 1;
}


