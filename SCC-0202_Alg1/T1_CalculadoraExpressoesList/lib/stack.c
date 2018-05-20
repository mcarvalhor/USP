#include <stdlib.h>
#include <assert.h>
#include "stack.h"

#ifndef STACK_ELEM
	#define STACK_ELEM int
#endif



/*
 *	~         STACK         ~
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



struct __stack_t { /* Estrutura da pilha. */
	int Top;
	STACK_ELEM *L;
};



struct __stack_t *S_New(){
	/*
	* Cria uma nova pilha, retornando-a.
	*/
	struct __stack_t *Aux=(struct __stack_t *)malloc(sizeof(struct __stack_t));
	Aux->Top=-1;
	Aux->L=NULL;
	return Aux;
}

struct __stack_t *S_NewFrom(struct __stack_t *S){
	/*
	* Cria uma nova pilha como cópia de 'S'.
	*
	* Retorna o endereço da nova pilha cópia, ou NULL em caso de erros.
	*/
	struct __stack_t *Aux;
	int i;
	if(S==NULL)	return NULL;
	Aux=(struct __stack_t *)malloc(sizeof(struct __stack_t));
	Aux->Top=S->Top;
	Aux->L=(STACK_ELEM *)malloc(sizeof(STACK_ELEM)*(S->Top+1));
	for(i=0;i<=S->Top;i++)	Aux->L[i]=S->L[i];
	return Aux;
}

char S_Cmp(struct __stack_t *A,struct __stack_t *B){
	/*
	* Esta função compara duas pilhas 'A' e 'B' em seus valores (como se fossem uma "string").
	*
	* Retorna 0 se forem pilhas iguais, -1 se 'A' for maior, 1 se 'B' for maior, e -2 em caso de erros.
	*/
	int i,G;
	if(A==NULL || B==NULL)	return -2;
	if(A->Top<=B->Top)	G=A->Top;
	else	G=B->Top;
	for(i=0;i<=G;i++){
		if(A->L[i]>B->L[i])	return -1;
		else if(A->L[i]<B->L[i])	return 1;
	}
	if(A->Top<B->Top)	return -1;
	else if(A->Top>B->Top)	return 1;
	return 0;
}

char S_IsEmpty(struct __stack_t *S){
	/*
	* Retorna 1 se a pilha 'S' estiver vazia, ou 0 em caso contrário.
	*
	* Retorna também -1 em casos de erros.
	*/
	if(S==NULL)	return -1;
	return (S->Top>-1) ? 0 : 1;
}

int S_Size(struct __stack_t *S){
	/*
	* Retorna o tamanho da pilha 'S', ou -1 em caso de erros.
	*/
	if(S==NULL)	return -1;
	return S->Top+1;
}

char S_Push(STACK_ELEM X,struct __stack_t *S){
	/*
	* Adiciona 'X' ao topo da pilha 'S'.
	*
	* Retorna 0 em caso de erros ou 1 em caso de sucesso.
	*/
	if(S==NULL)	return 0;
	S->L=(STACK_ELEM *)realloc(S->L,sizeof(STACK_ELEM)*(S->Top+2));
	S->Top++;
	S->L[S->Top]=X;
	return 1;
}

STACK_ELEM S_Pop(struct __stack_t *S){
	/*
	* Retorna o último elemento da pilha 'S', retirando-o dela.
	*
	* ATENÇÃO: Em caso de erros, essa função PARA a execução.
	*/
	STACK_ELEM Aux;
	if(S==NULL)	assert(0); /* Erro: ponteiro nulo. */
	if(S->Top<0)	assert(0); /* Erro: pilha vazia. */
	Aux=S->L[S->Top];
	S->L=(STACK_ELEM *)realloc(S->L,sizeof(STACK_ELEM)*S->Top);
	S->Top--;
	return Aux;
}

STACK_ELEM S_Get(struct __stack_t *S){
	/*
	* Retorna o último elemento da pilha 'S', sem retirar este dela.
	*
	* ATENÇÃO: Em caso de erros, essa função PARA a execução.
	*/
	if(S==NULL)	assert(0); /* Erro: ponteiro nulo. */
	if(S->Top<0)	assert(0); /* Erro: pilha vazia. */
	return S->L[S->Top];
}

STACK_ELEM S_GetAt(int i,struct __stack_t *S){
	/*
	* Retorna o elemento 'i' da pilha 'S', sem retirar este dela.
	*
	* ATENÇÃO: Em caso de erros, essa função PARA a execução.
	*/
	if(S==NULL)	assert(0); /* Erro: ponteiro nulo. */
	if(S->Top<0)	assert(0); /* Erro: pilha vazia. */
	if(i<0 || i>S->Top)	assert(0); /* Erro: posição "i" fora do intervalo da pilha. */
	return S->L[i];
}

char S_Destroy(struct __stack_t *S){
	/*
	* Limpa da memória a pilha 'S'.
	*/
	if(S==NULL)	return 0;
	if(S->L!=NULL)	free(S->L);
	free(S);
	return 1;
}



