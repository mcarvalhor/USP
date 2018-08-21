#include <stdlib.h>
#include <stdio.h>



/*
 *      ==         ABB         ==
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



struct __bin_search_node { // Nó da árvore.
	int Value;
	struct __bin_search_node *L,*R;
};

struct __bin_search_tree { // Estrutura da árvore.
	struct __bin_search_node *Root;
};



struct __bin_search_tree *Create_Tree(){
	/*
	* Esta função vai criar uma árvore binária de busca.
	*
	* Ela retorna um ponteiro para a árvore criada.
	*/
	struct __bin_search_tree *Aux=(struct __bin_search_tree *)malloc(sizeof(struct __bin_search_tree));
	Aux->Root=NULL;
	return Aux;
}

char Find_In_Node(int X, struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Find_In".
	*/
	if(N==NULL)	return -1;
	if(X==N->Value)	return 1;
	if(X<N->Value && N->L!=NULL && Find_In_Node(X,N->L)!=0)	return 1;
	if(X>N->Value && N->R!=NULL && Find_In_Node(X,N->R)!=0)	return 1;
	return 0;
}

char Find_In(int X, struct __bin_search_tree *T){
	/*
	* Esta função procura por 'X' na árvore binária de busca 'T'.
	*
	* Ela retorna 1 se 'X' existe na árvore, 0 se não existe, ou -1 em caso de erros.
	*/
	if(T==NULL)	return -1;
	if(T->Root!=NULL && Find_In_Node(X,T->Root)!=0)	return 1;
	return 0;
}

char Insert_Into_Node(int X, struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Insert_Into".
	*/
	if(N==NULL)	return 0;
	if(X<=N->Value){
		if(N->L!=NULL)	return Insert_Into_Node(X,N->L);
		N->L=(struct __bin_search_node *)malloc(sizeof(struct __bin_search_node));
		N->L->Value=X;
		N->L->L=N->L->R=NULL;
		return 1;
	}
	if(N->R!=NULL)	return Insert_Into_Node(X,N->R);
	N->R=(struct __bin_search_node *)malloc(sizeof(struct __bin_search_node));
	N->R->Value=X;
	N->R->L=N->R->R=NULL;
	return 1;
};

char Insert_Into(int X, struct __bin_search_tree *T){
	/*
	* Esta função insere na árvore binária de busca 'T' o valor 'X'.
	*
	* Ela retorna 1 se inseriu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	if(T->Root!=NULL)	return Insert_Into_Node(X,T->Root);
	T->Root=(struct __bin_search_node *)malloc(sizeof(struct __bin_search_node));
	T->Root->Value=X;
	T->Root->L=T->Root->R=NULL;
	return 1;
}

char Remove_First_From_Node(int X, struct __bin_search_node **N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Remove_First_From".
	*/
	if(N==NULL || *N==NULL)	return -1;
	struct __bin_search_node *Aux;
	if(X==(*N)->Value){
		Aux=*N;
		if((*N)->L==NULL)	*N=(*N)->R;
		else if((*N)->R==NULL)	*N=(*N)->L;
		else{
			struct __bin_search_node **R=&(*N)->R;
			while((*R)->L!=NULL){
				R=&(*R)->L;
			}
			Aux=*R;
			(*N)->Value=(*R)->Value;
			*R=(*R)->R;
		}
		free(Aux);
		return 1;
	}
	if(X<(*N)->Value && (*N)->L!=NULL && Remove_First_From_Node(X,&(*N)->L)==1)	return 1;
	if(X>(*N)->Value && (*N)->R!=NULL && Remove_First_From_Node(X,&(*N)->R)==1)	return 1;
	return 0;
}

char Remove_First_From(int X, struct __bin_search_tree *T){
	/*
	* Esta função remove a primeira ocorrência encontrada de 'X' da árvore binária de busca 'T'.
	*
	* Ela retorna 1 se 'X' foi removido, 0 se 'X' não existe na árvore, ou -1 em caso de erros.
	*/
	if(T==NULL)	return -1;
	if(T->Root!=NULL && Remove_First_From_Node(X,&T->Root)==1)	return 1;
	return 0;
}

char Print_Node_InOrder(FILE *FStream,struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Print_InOrder".
	*/
	if(N==NULL)	return 0;
	Print_Node_InOrder(FStream,N->L);
	fprintf(FStream," %d",N->Value);
	Print_Node_InOrder(FStream,N->R);
	return 1;
}

char Print_InOrder(FILE *FStream,struct __bin_search_tree *T){
	/*
	* Esta função imprime em 'FStream' toda a árvore binária de busca 'T'.
	* Note que ela vai imprimir da forma "em ordem", ou seja, primeiro os filhos da esquerda, a chave, e finalmente os filhos da direita.
	*
	* Ela retorna 1 se imprimiu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	fprintf(FStream,"InOrdem:");
	Print_Node_InOrder(FStream,T->Root);
	printf("\n");
	return 1;
}

char Print_Node_PreOrder(FILE *FStream,struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Print_PreOrder".
	*/
	if(N==NULL)	return 0;
	fprintf(FStream," %d",N->Value);
	Print_Node_PreOrder(FStream,N->L);
	Print_Node_PreOrder(FStream,N->R);
	return 1;
}

char Print_PreOrder(FILE *FStream,struct __bin_search_tree *T){
	/*
	* Esta função imprime em 'FStream' toda a árvore binária de busca 'T'.
	* Note que ela vai imprimir da forma "pré-ordem", ou seja, primeiro a chave, os filhos da esquerda, e finalmente os filhos da direita.
	*
	* Ela retorna 1 se imprimiu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	fprintf(FStream,"PreOrdem:");
	Print_Node_PreOrder(FStream,T->Root);
	printf("\n");
	return 1;
}

char Print_Node_PostOrder(FILE *FStream,struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Print_PostOrder".
	*/
	if(N==NULL)	return 0;
	Print_Node_PostOrder(FStream,N->L);
	Print_Node_PostOrder(FStream,N->R);
	fprintf(FStream," %d",N->Value);
	return 1;
}

char Print_PostOrder(FILE *FStream,struct __bin_search_tree *T){
	/*
	* Esta função imprime em 'FStream' toda a árvore binária de busca 'T'.
	* Note que ela vai imprimir da forma "pós-ordem", ou seja, primeiro os filhos da esquerda, os filhos da direita, e finalmente a chave.
	*
	* Ela retorna 1 se imprimiu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	fprintf(FStream,"PosOrdem:");
	Print_Node_PostOrder(FStream,T->Root);
	printf("\n");
	return 1;
}

char Print_Node_LevelOrder(FILE *FStream,int Level,struct __bin_search_node *N){ // Cuidado: complexidade O(n²).
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Print_LevelOrder".
	*/
	if(N==NULL)	return 0;
	char R;
	if(Level<=1){
		fprintf(FStream," %d",N->Value); fflush(FStream);
		return 1;
	}
	Level--;
	R=Print_Node_LevelOrder(FStream,Level,N->L);
	if(Print_Node_LevelOrder(FStream,Level,N->R)!=1 && R!=1)	return 0;
	return 1;
}

char Print_LevelOrder(FILE *FStream,struct __bin_search_tree *T){
	/*
	* Esta função imprime em 'FStream' toda a árvore binária de busca 'T'.
	* Note que ela vai imprimir da forma "largura", ou seja, todos os elementos do nível 1, 2, 3, ..., e n, sendo n o último nível da árvore.
	*
	* Ela retorna 1 se imprimiu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	int i=1;
	fprintf(FStream,"Largura:");
	while(Print_Node_LevelOrder(FStream,i,T->Root)==1)	i++;
	printf("\n");
	return 1;
}

char Destroy_Node(struct __bin_search_node *N){
	/*
	* Esta é uma função auxiliar (não disponível abertamente na biblioteca) para "Destroy_Tree".
	*/
	if(N==NULL)	return 0;
	Destroy_Node(N->L);
	Destroy_Node(N->R);
	free(N);
	return 1;
}

char Destroy_Tree(struct __bin_search_tree *T){
	/*
	* Esta função vai remover da memória toda a árvore binária de busca 'T'.
	*
	* Ela retorna 1 se removeu com sucesso, ou 0 em caso de erros.
	*/
	if(T==NULL)	return 0;
	Destroy_Node(T->Root);
	free(T);
	return 1;
}


