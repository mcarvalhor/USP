#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <linkedlist.h>
#include <super_file.h>
#include "stack.h"



/*
 *	~  Calc. de Expressões  ~
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



enum __bool_t { false=0, true }; /* Pseudo-booleano */



LIST *NewExpression(FILE *FStream){
	/*
	* Essa função lê uma expressão matemática de 'FStream' e retorna um ponteiro para tal.
	*
	* Ela também pode retornar NULL caso 'FStream' não contenha uma expressão matemática válida.
	*/
	enum __bool_t valid=true;
	LIST *Expression=L_New();
	STACK *Lists=S_New();
	double S;
	char R;
	int i;
	while( (R=SF_PreviewChar(FStream))!=',' && R!=';' ){
		if(R>='0' && R<='9'){ /* É um número real. */
			fscanf(FStream,"%lf",&S);
			L_Add(S,Expression); /* Adicionar à lista. */
		}else if(R=='('){ /* É uma expressão entre parênteses. */
			getc(FStream); /* Consumir parênteses. */
			S_Push(Expression,Lists); /* Empilhar para salvar expressão dentro dos parênteses. */
			Expression=L_New();
			L_AddSubList(Expression,S_Get(Lists));
		}else if(R==')'){
			getc(FStream); /* Consumir fechamento de parênteses */
			if(S_Size(Lists)<=0){ /* Esse fechamento de parênteses não é esperado aí. */
				valid=false;
				break;
			}else{
				Expression=S_Pop(Lists); /* Desempilhar */
			}
		}else if(R=='+' || R=='-' || R=='*' || R=='/'){ /* É um operador. */
			getc(FStream); /* Consumir operador. */
			L_Add(R*-1,Expression); /* Os operadores são representados por números negativos. */
		}else if(isspace(R)){ /* É um caractere de espaçamento. */
			getc(FStream); /* Apenas consumir e ignorar tal. */
		}else{
			valid=false;
			break;
		}
	}
	if(S_Size(Lists)>0)	valid=false;
	if(valid==false){
		if(S_Size(Lists)>0)	L_Destroy(S_GetAt(0,Lists));
		else	L_Destroy(Expression);
		S_Destroy(Lists);
		return NULL;
	}
	S_Destroy(Lists);
	for(i=0;i<L_Size(Expression);i++){ /* Checar se a expressão possui o padrão "(Numero/SubExpressao Operador Numero/SubExpressao) ? " */
		if(i%2){
			if(L_IsSubListAt(i,Expression) || L_GetAt(i,Expression)>=0){
				L_Destroy(Expression);
				return NULL;
			}
		}else{
			if(!L_IsSubListAt(i,Expression) && L_GetAt(i,Expression)<0){
				L_Destroy(Expression);
				return NULL;
			}
		}
	}
	return Expression;
}

char CalcExpression(double *SetVar,LIST *Exp){
	/*
	* Esta função calcula o resultado final da expressão 'Exp' e salva em 'SetVar'.
	*
	* Ela retorna 1 em caso de sucesso, -2 em caso de erros de ponteiro, -1 em caso de uma lista vazia, 0 em caso de divisão por zero.
	*/
	double R;
	int i;
	if(Exp==NULL)	return -2; /* Erro: Lista não pode ser NULL. */
	if(L_Size(Exp)<1)	return -1; /* Erro: Lista não possui nenhuma expressão válida. */
	for(i=0;i<L_Size(Exp);i+=2) /* Percorrer lista resolvendo tudo o que está dentro de parênteses. */
		if(L_IsSubListAt(i,Exp)){
			if(CalcExpression(&R,L_GetSubListAt(i,Exp))!=1){
				return 0;
			}
			L_SetAt(R,i,Exp);
		}
	for(i=L_Size(Exp)-2;i>0;i-=2) /* Percorrer lista resolvendo operações prioritárias, como multiplicação e divisão. */
		if(L_GetAt(i,Exp)=='/'*-1){
			if(L_GetAt(i+1,Exp)==0){
				return 0;	/* Divisão por zero. */
			}
			L_SetAt(L_GetAt(i-1,Exp)/L_GetAt(i+1,Exp),i,Exp);
			L_RemoveAt(i+1,Exp);
			L_RemoveAt(i-1,Exp);
		}else if(L_GetAt(i,Exp)=='*'*-1){
			L_SetAt(L_GetAt(i-1,Exp)*L_GetAt(i+1,Exp),i,Exp);
			L_RemoveAt(i+1,Exp);
			L_RemoveAt(i-1,Exp);
		}
	for(i=L_Size(Exp)-2;i>0;i-=2) /* Agora resolva operações triviais, como soma e subtração. */
		if(L_GetAt(i,Exp)=='+'*-1){
			L_SetAt(L_GetAt(i-1,Exp)+L_GetAt(i+1,Exp),i,Exp);
			L_RemoveAt(i+1,Exp);
			L_RemoveAt(i-1,Exp);
		}else if(L_GetAt(i,Exp)=='-'*-1){
			L_SetAt(L_GetAt(i-1,Exp)-L_GetAt(i+1,Exp),i,Exp);
			L_RemoveAt(i+1,Exp);
			L_RemoveAt(i-1,Exp);
		}
	*SetVar=L_GetAt(0,Exp); /* O resultado está no primeiro e único elemento agora. */
	return 1;
}

char DestroyExpression(LIST *Exp){
	/*
	* Essa função remove da memória a lista 'Exp'.
	* No fundo, ela apenas chama a própria função do TAD responsável por remover listas.
	*
	* Ela retorna 1 em caso de sucesso, outros valores caso contrário.
	*/
	return L_Destroy(Exp);
}



