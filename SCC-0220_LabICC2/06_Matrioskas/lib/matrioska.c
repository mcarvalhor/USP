#include <stdio.h>
#include "stack.h"



/*
 *	~       MATRIOSKA       ~
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



enum __Matrioska_LastStackAction_t { Popped = 0, Pushed };



int Matrioska_Check(FILE *FStream){
	/*
	* Esta função verifica se em 'FStream' há uma sequência que descreve uma "Matrioska" aguardando para ser lida.
	*
	* Ela retorna o número de Matrioskas possíveis de ser criadas com a sequência, ou -1 em caso de erros de empilhamento, e -2 em caso de erros de chegar ao fim sem terminar de fato a sequência.
	*/
	STACK *DoolStack;
	int N=0,R;
	enum __Matrioska_LastStackAction_t Last=Popped;
	fscanf(FStream,"%d",&R);
	if(R==0){
		return 0; // Começou finalizando.
	}else if(R>0){
		return -2; // O primeiro valor da sequência já indica que não é uma Matroska.
	}
	DoolStack=S_New();
	S_Push(R,DoolStack);
	while(R!=0){
		if(R<0){
			if(S_Size(DoolStack)>0 && Last==Pushed && R<S_Get(DoolStack)){
				S_Destroy(DoolStack);
				return -2;
			}
			S_Push(R,DoolStack);
			if(Last==Popped){
				N++;
			}
			Last=Pushed;
		}else if(R>0){
			if(S_Size(DoolStack)<=0 || (R*-1)!=S_Get(DoolStack)){
				S_Destroy(DoolStack);
				return -2;
			}
			S_Pop(DoolStack);
			Last=Popped;
		}
		fscanf(FStream,"%d",&R);
	}
	if(S_Size(DoolStack)!=1){
		S_Destroy(DoolStack);
		return -1;
	}
	S_Destroy(DoolStack);
	return N; // Sucesso na sequência.
}



