#include <stdlib.h>
#include <assert.h>
#include "linkedlist.h"

#ifndef LINKEDLIST_ELEM
	#define LINKEDLIST_ELEM int
#endif



/*
 *      ~    Lista Enumerada    ~
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



enum __ListItem_Type_t { Elem = 0, List };


union __ListItem_Value_t {
	LINKEDLIST_ELEM E;
	struct __List_t *L;
};


struct __ListItem_t{
	enum __ListItem_Type_t Type;
	union __ListItem_Value_t Value;
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

char L_Add(LINKEDLIST_ELEM X,struct __List_t *L){
	struct __ListItem_t *Aux;
	if(L==NULL)	return 0;
	if(L->size==0){
		L->end=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
		L->end->Type=Elem;
		L->end->Value.E=X;
		L->end->N=NULL;
		L->size++;
		L->start=L->end;
		return 2;
	}
	Aux=L->end;
	Aux->N=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	Aux->N->Type=Elem;
	Aux->N->Value.E=X;
	Aux->N->N=NULL;
	L->end=Aux->N;
	L->size++;
	return 1;
}

char L_AddSubList(struct __List_t *X,struct __List_t *L){
	struct __ListItem_t *Aux;
	if(L==NULL)	return 0;
	if(L->size==0){
		L->end=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
		L->end->Type=List;
		L->end->Value.L=X;
		L->end->N=NULL;
		L->size++;
		L->start=L->end;
		return 2;
	}
	Aux=L->end;
	Aux->N=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	Aux->N->Type=List;
	Aux->N->Value.L=X;
	Aux->N->N=NULL;
	L->end=Aux->N;
	L->size++;
	return 1;
}

char L_AddAt(LINKEDLIST_ELEM X,int i,struct __List_t *L){
	struct __ListItem_t *Aux,*NewAux;
	int p;
	if(L==NULL)	return -1;
	if(i<0 || i>L->size)	return 0;
	Aux=L->start;
	NewAux=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	if(L->size==0){
		NewAux->N=NULL;
		L->start=L->end=NewAux;
	}else if(i==L->size){
		NewAux->N=NULL;
		L->end->N=NewAux;
		L->end=NewAux;
	}else if(i==L->size-1){
		NewAux->N=L->end->N;
		L->end->N=NewAux;
		L->end=NewAux;
	}else if(i==0){
		NewAux->N=L->start;
		L->start=NewAux;
	}else{
		p=0;
		while(p<i-1){
			Aux=Aux->N;
			p++;
		}
		NewAux->N=Aux->N->N;
		Aux->N=NewAux;
	}
	NewAux->Type=Elem;
	NewAux->Value.E=X;
	L->size++;
	return 1;
}

char L_AddSubListAt(struct __List_t *X,int i,struct __List_t *L){
	struct __ListItem_t *Aux,*NewAux;
	int p;
	if(L==NULL)	return -1;
	if(i<0 || i>L->size)	return 0;
	Aux=L->start;
	NewAux=(struct __ListItem_t *)malloc(sizeof(struct __ListItem_t)*1);
	if(L->size==0){
		NewAux->N=NULL;
		L->start=L->end=NewAux;
	}else if(i==L->size){
		NewAux->N=NULL;
		L->end->N=NewAux;
		L->end=NewAux;
	}else if(i==L->size-1){
		NewAux->N=L->end->N;
		L->end->N=NewAux;
		L->end=NewAux;
	}else if(i==0){
		NewAux->N=L->start;
		L->start=NewAux;
	}else{
		p=0;
		while(p<i-1){
			Aux=Aux->N;
			p++;
		}
		NewAux->N=Aux->N->N;
		Aux->N=NewAux;
	}
	NewAux->Type=List;
	NewAux->Value.L=X;
	L->size++;
	return 1;
}

char L_Destroy(struct __List_t *);

char L_Remove(struct __List_t *L){
	struct __ListItem_t *Aux;
	if(L==NULL)	return -1;
	if(L->size<=0)	return 0;
	Aux=L->start;
	while(Aux->N!=L->end && Aux->N!=NULL){
		Aux=Aux->N;
	}
	Aux->N=NULL;
	if(L->end->Type==List)	L_Destroy(L->end->Value.L);
	free(L->end);
	L->end=Aux;
	L->size--;
	if(L->size<=0){
		L->end=L->start=NULL;
	}
	return 1;
}

char L_RemoveAt(int i,struct __List_t *L){
	struct __ListItem_t *Aux,*R;
	int p;
	if(L==NULL)	return -1;
	if(i<0 || i>=L->size)	return 0;
	Aux=L->start;
	if(L->size==1){
		if(Aux->Type==List)	L_Destroy(Aux->Value.L);
		free(Aux);
		L->start=NULL;
		L->end=NULL;
	}else if(i==0){
		R=Aux->N;
		if(Aux->Type==List)	L_Destroy(Aux->Value.L);
		free(Aux);
		L->start=R;
	}else{
		for(p=0;p<i-1;p++)
			Aux=Aux->N;
		R=Aux->N;
		Aux->N=Aux->N->N;
		if(i==L->size-1){
			L->end=Aux;
		}
		if(R->Type==List)	L_Destroy(R->Value.L);
		free(R);
	}
	L->size--;
	return 1;
}

char L_IsSubList(struct __List_t *L){
	if(L==NULL)	return -2;
	if(L->size<=0)	return -1;
	return (L->end->Type==List) ? 1 : 0;
}

char L_IsSubListAt(int i,struct __List_t *L){
	struct __ListItem_t *Aux;
	int p;
	if(L==NULL)	return -2;
	if(i<0 || i>=L->size)	return -1;
	if(i==L->size-1){
		Aux=L->end;
		return (Aux->Type==List) ? 1 : 0;
	}
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	return (Aux->Type==List) ? 1 : 0;
}

LINKEDLIST_ELEM L_Get(struct __List_t *L){
	if(L==NULL)	assert(0);
	if(L->size<=0)	assert(0);
	return L->end->Value.E;
}

LINKEDLIST_ELEM L_GetAt(int i,struct __List_t *L){
	struct __ListItem_t *Aux;
	int p;
	if(L==NULL)	assert(0);
	if(i<0 || i>=L->size)	assert(0);
	if(i==L->size-1){
		Aux=L->end;
		return Aux->Value.E;
	}
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	return Aux->Value.E;
}

struct __List_t *L_GetSubList(struct __List_t *L){
	if(L==NULL)	assert(0);
	if(L->size<=0)	assert(0);
	return L->end->Value.L;
}

struct __List_t *L_GetSubListAt(int i,struct __List_t *L){
	struct __ListItem_t *Aux;
	int p;
	if(L==NULL)	assert(0);
	if(i<0 || i>=L->size)	assert(0);
	if(i==L->size-1){
		Aux=L->end;
		return Aux->Value.L;
	}
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	return Aux->Value.L;
}

char L_Set(LINKEDLIST_ELEM X,struct __List_t *L){
	if(L==NULL)	return -1;
	if(L->size<=0)	return 0;
	if(L->end->Type==List)	L_Destroy(L->end->Value.L);
	L->end->Type=Elem;
	L->end->Value.E=X;
	return 1;
}

char L_SetAt(LINKEDLIST_ELEM X,int i,struct __List_t *L){
	struct __ListItem_t *Aux;
	int p;
	if(L==NULL)	return -1;
	if(i<0 || i>=L->size)	return 0;
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	if(Aux->Type==List)	L_Destroy(Aux->Value.L);
	Aux->Type=Elem;
	Aux->Value.E=X;
	return 1;
}

char L_SetSubList(struct __List_t *X,struct __List_t *L){
	if(L==NULL)	return -1;
	if(L->size<=0)	return 0;
	if(L->end->Type==List)	L_Destroy(L->end->Value.L);
	L->end->Type=List;
	L->end->Value.L=X;
	return 1;
}

char L_SetSubListAt(struct __List_t *X,int i,struct __List_t *L){
	struct __ListItem_t *Aux;
	int p;
	if(L==NULL)	return -1;
	if(i<0 || i>=L->size)	return 0;
	Aux=L->start;
	for(p=0;p<i;p++)
		Aux=Aux->N;
	if(Aux->Type==List)	L_Destroy(Aux->Value.L);
	Aux->Type=List;
	Aux->Value.L=X;
	return 1;
}

char L_Destroy(struct __List_t *L){
	struct __ListItem_t *Aux,*R;
	if(L==NULL)	return 0;
	if(L->size>0){
		Aux=L->start;
		while(Aux!=NULL){
			R=Aux->N;
			if(Aux->Type==List)	L_Destroy(Aux->Value.L);
			free(Aux);
			Aux=R;
		}
	}
	free(L);
	return 1;
}



