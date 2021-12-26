#include <stdio.h>
#include <list.h>



/*
 *	~         Cartas        ~
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



LIST *ReadCardsFrom(FILE *FStream){
	/*
	* Esta função lê de 'FStream' um conjunto de cartas.
	*
	* Ela retorna um ponteiro para a lista que representa esse conjunto, ou NULL em caso de erros.
	*/
	LIST *Aux=L_New();
	int R=0;
	scanf("%d",&R);
	while(R!=0){
		L_Add(R,Aux);
		scanf("%d",&R);
	}
	if(L_Size(Aux)<=0){
		L_Destroy(Aux);
		return NULL;
	}
	return Aux;
}

char ProcessCards(FILE *FStream, LIST *Cards){
	/*
	* Esta função processa as cartas da lista 'Cards' e imprime o resultado em 'FStream'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(Cards==NULL || L_Size(Cards)<=0)	return 0;
	fprintf(FStream,"Cartas jogadas:");
	while(L_Size(Cards)>1){
		fprintf(FStream," %d",L_GetAt(0,Cards));
		L_RemoveAt(0,Cards);
		L_Add(L_GetAt(0,Cards),Cards);
		L_RemoveAt(0,Cards);
	}
	fprintf(FStream,"\nCarta restante: %d",L_Get(Cards));
	return 1;
}

char DestroyCards(LIST *Cards){
	return L_Destroy(Cards);
}



