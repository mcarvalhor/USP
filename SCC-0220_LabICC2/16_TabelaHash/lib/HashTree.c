#include <stdlib.h>
#include <stdio.h>



/*
 *	~      Tabela Hash      ~
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



struct __tree_node_t {
	int element;
	struct __tree_node_t *l, *r;
};

struct __hash_node_t{
	struct __tree_node_t *Root;
	unsigned long N;
};

struct __hashtree_t{
	struct __hash_node_t *Table;
	unsigned long M, N;
};



struct __hashtree_t *HTree_New(unsigned long M){
	if(M < 1)	return NULL;
	struct __hashtree_t *Aux=(struct __hashtree_t *)malloc(sizeof(struct __hashtree_t));
	Aux->Table=(struct __hash_node_t *)calloc(M, sizeof(struct __hash_node_t));
	Aux->M=M;
	Aux->N=0;
	return Aux;
}

unsigned long HTree_HashFunction(int element, struct __hashtree_t *T){
	unsigned long Key;
	Key=	element%T->M;
	return Key;
}

int HTree_Insert(int element, struct __hashtree_t *T){
	if(T==NULL)	return 0;
	unsigned long Pos=HTree_HashFunction(element, T);
	struct __tree_node_t **Aux=&T->Table[Pos].Root;
	while((*Aux)!=NULL){
		if(element < (*Aux)->element)	Aux=&(*Aux)->l;
		else	Aux=&(*Aux)->r;
	}
	*Aux = (struct __tree_node_t *)calloc(1, sizeof(struct __tree_node_t));
	(*Aux)->element=element;
	T->Table[Pos].N++;
	T->N++;
	return 1;
}

int HTree_Remove(int element, struct __hashtree_t *T){
	if(T==NULL)	return -1;
	unsigned long Pos=HTree_HashFunction(element, T);
	struct __tree_node_t **Aux=&T->Table[Pos].Root, **Aux1, *Aux2;
	while((*Aux)!=NULL){
		if(element < (*Aux)->element)	Aux=&(*Aux)->l;
		else if(element > (*Aux)->element)	Aux=&(*Aux)->r;
		else	break;
	}
	if((*Aux)==NULL)	return 0;
	if((*Aux)->l==NULL){
		Aux2=*Aux;
		*Aux=(*Aux)->r;
		free(Aux2);
	}else if((*Aux)->r==NULL){
		Aux2=*Aux;
		*Aux=(*Aux)->l;
		free(Aux2);
	}else{
		Aux1=&(*Aux)->r;
		while((*Aux1)->l!=NULL)	Aux1=&(*Aux1)->l;
		Aux2=*Aux1;
		(*Aux)->element=Aux2->element;
		free(Aux2);
		*Aux1=NULL;
	}
	T->Table[Pos].N--;
	T->N--;
	return 1;
}

int HTree_Count(int element, struct __hashtree_t *T){
	if(T==NULL)	return 0;
	unsigned long Pos=HTree_HashFunction(element, T);
	return T->Table[Pos].N;
}

void HTree_Traverse_(FILE *FStream, struct __tree_node_t *Root, unsigned long *N){
	if(Root==NULL)	return;
	HTree_Traverse_(FStream, Root->l, N);
	fprintf(FStream, "%d", Root->element);
	if(*N>1)	fprintf(FStream, ", ");
	(*N)--;
	HTree_Traverse_(FStream, Root->r, N);
}

int HTree_Traverse(FILE *FStream, struct __hashtree_t *T){
	if(T==NULL)	return 0;
	if(T->N < 1)	return 1;
	unsigned long i, Aux=T->Table[0].N;
	char PrintComma=0;
	for(i=1;i<T->M;i++){
		Aux=T->Table[i].N;
		if(T->Table[i].N > 1){
			if(PrintComma)	printf(", ");
			PrintComma=1;
			fprintf(FStream, "(");
			HTree_Traverse_(FStream, T->Table[i].Root, &Aux);
			fprintf(FStream, ")");
		}else if(T->Table[i].N == 1){
			if(PrintComma)	printf(", ");
			PrintComma=1;
			HTree_Traverse_(FStream, T->Table[i].Root, &Aux);
		}
	}
	return 2;
}

void HTree_Destroy_(struct __tree_node_t *Root){
	if(Root==NULL)	return;
	HTree_Destroy_(Root->l);
	HTree_Destroy_(Root->r);
	free(Root);
}

void HTree_Destroy(struct __hashtree_t *T){
	if(T==NULL)	return;
	unsigned long i;
	for(i=0;i<T->M;i++)
		HTree_Destroy_(T->Table[i].Root);
	free(T->Table);
	free(T);
}



