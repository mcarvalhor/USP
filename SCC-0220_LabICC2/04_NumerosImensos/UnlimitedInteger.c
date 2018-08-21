#include <stdlib.h>
#include <stdio.h>
#include "List.h"


/*
 *      == Inteiros Ilimitados ==
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


enum __UI_bool_t { false = 0, true };

struct __UI_t {
	enum __UI_bool_t Negative;
	LIST *Number;
};


struct __UI_t *UI_New(){
	/*
	* Esta função retorna o número zero.
	*/
	struct __UI_t *NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t *));
	NewUI->Negative=false;
	NewUI->Number=L_New();
	return NewUI;
}

struct __UI_t *UI_NewFromInt(long int NumberInt){
	/*
	* Esta função vai criar um número a partir de um inteiro padrão (long int) e retornar tal.
	*/
	struct __UI_t *NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t));
	char R[30],RX[2]="0";
	int i=0,j;
	NewUI->Number=L_New();
	if(NumberInt>=0)	NewUI->Negative=false;
	else	NewUI->Negative=true;
	if(NumberInt==0)	return NewUI;
	sprintf(R,"%ld",(unsigned long int)NumberInt);
	while(R[i]!='\0')	i++; // i=strlen(R);
	for(j=i-1;j>=0;j--){
		RX[0]=R[j];
		L_Add((char)atoi(RX),NewUI->Number);
	}
	return NewUI;
}

struct __UI_t *UI_Read(FILE *FStream){
	/*
	* Esta função vai ler um número de 'FStream' e retornar tal.
	* Ela funciona para números negativos.
	*
	* Em caso de leitura impossibilitada, irá retornar o número '0'.
	*/
	char R,RX[2]="0",*NewNumber=(char *)malloc(sizeof(char *)*1);
	struct __UI_t *NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t));
	int i,Len=0;
	NewUI->Negative=false;
	while( !((R=getc(FStream))>='0' && R<='9') ){
		if(R=='-')	NewUI->Negative=true;
		if(R==EOF)	break;
	}
	while( R>='0' && R<='9'){
		NewNumber=(char *)realloc(NewNumber,sizeof(char)*(Len+2));
		NewNumber[Len++]=R;
		R=getc(FStream);
	}
	NewNumber[Len]='\0';
	NewUI->Number=L_New();
	if(Len>0){
		for(i=Len-1;i>=0;i--){
			RX[0]=NewNumber[i];
			L_Add((char)atoi(RX),NewUI->Number);
		}
	}else{
		NewUI->Negative=false;
	}
	for(i=L_Size(NewUI->Number)-1;i>=0;i--){
		if(L_Get(NewUI->Number)==0)	L_Remove(NewUI->Number);
		else	break;
	}
	free(NewNumber);
	return NewUI;
}

char UI_Write(FILE *FStream, struct __UI_t *N){
	/*
	* Esta função vai escrever um número em 'FStream'.
	*
	* Retorna -1 em caso de erros, 0 quando o número escrito é o próprio zero, e 1 em caso de escrita de qualquer outro número.
	*/
	if(N->Number==NULL)	return -1;
	int i,Len=L_Size(N->Number);
	if(Len<=0){
		fprintf(FStream,"0");
		return 0;
	}
	if(N->Negative==true)	fprintf(FStream,"-");
	for(i=Len-1;i>=0;i--){
		fprintf(FStream,"%d",(int)L_GetAt(i,N->Number));
	}
	return 1;
}

char UI_Cmp(struct __UI_t *A, struct __UI_t *B){
	/*
	* Esta função compara 'A' com 'B'.
	*
	* Retorna -2 em caso de erros, -1 para 'A' maior que 'B', 0 para 'A' igual a 'B', e 1 para 'A' menor que 'B'.
	*/
	if(A->Number==NULL || B->Number==NULL)	return -2;
	if(A->Negative==false && B->Negative==true)	return -1;
	if(A->Negative==true && B->Negative==false)	return 1;
	int i,ALen=L_Size(A->Number),BLen=L_Size(B->Number);
	char AR,BR;
	if(ALen>BLen)	return -1;
	if(ALen<BLen)	return 1;
	if(A->Negative==false)
		for(i=ALen-1;i>=0;i--){
			AR=L_GetAt(i,A->Number);
			BR=L_GetAt(i,B->Number);
			if(AR>BR)	return -1;
			if(AR<BR)	return 1;
		}
	else
		for(i=ALen-1;i>=0;i--){
			AR=L_GetAt(i,A->Number);
			BR=L_GetAt(i,B->Number);
			if(AR>BR)	return 1;
			if(AR<BR)	return -1;
		}
	return 0;
}

struct __UI_t *UI_Abs(struct __UI_t *N){
	/*
	* Esta função retorna o valor absoluto (módulo) de 'N'.
	*
	* Em caso de erros, retorna NULL.
	*/
	if(N==NULL || N->Number==NULL)	return NULL;
	struct __UI_t *NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t));
	int i;
	NewUI->Negative=false;
	NewUI->Number=L_New();
	for(i=0;i<L_Size(N->Number);i++)	L_Add(L_GetAt(i,N->Number),NewUI->Number);
	return NewUI;
}

struct __UI_t *UI_Subtraction(struct __UI_t *,struct __UI_t *); // Declaração explícita.
char UI_Destroy(struct __UI_t *); // Declaração explícita.

struct __UI_t *UI_Sum(struct __UI_t *A,struct __UI_t *B){
	/*
	* Esta função retorna a soma de 'A' com 'B' (A+B).
	*
	* Em caso de erros, retorna NULL.
	*/
	if(A==NULL || A->Number==NULL || B==NULL || B->Number==NULL)	return NULL;
	struct __UI_t *RUI,*NewUI;
	if(A->Negative==false && B->Negative==true){ // A-B
		RUI=UI_Abs(B);
		NewUI=UI_Subtraction(A,RUI);
		UI_Destroy(RUI);
		return NewUI;
	}
	if(A->Negative==true &&  B->Negative==false){ // B-A
		RUI=UI_Abs(A);
		NewUI=UI_Subtraction(B,RUI);
		UI_Destroy(RUI);
		return NewUI;
	}
	int i,ALen,BLen,Len;
	char R=0;
	NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t));
	NewUI->Number=L_New();
	if(A->Negative==false && B->Negative==false)	NewUI->Negative=false; // A+B
	else if(A->Negative==true && A->Negative==true)	NewUI->Negative=true; // A+B
	ALen=L_Size(A->Number);
	BLen=L_Size(B->Number);
	if(ALen<=BLen)	Len=ALen;
	else	Len=BLen;
	for(i=0;i<Len;i++){
		R+=L_GetAt(i,A->Number)+L_GetAt(i,B->Number);
		L_Add(R%10,NewUI->Number);
		R/=10;
	}
	if(ALen<BLen){
		for(i=ALen;i<BLen;i++){
			R+=L_GetAt(i,B->Number);
			L_Add(R%10,NewUI->Number);
			R/=10;
		}
	}else if(ALen>BLen){
		for(i=BLen;i<ALen;i++){
			R+=L_GetAt(i,A->Number);
			L_Add(R%10,NewUI->Number);
			R/=10;
		}
	}
	while(R>0){
		L_Add(R%10,NewUI->Number);
		R/=10;
	}
	return NewUI;
}

struct __UI_t *UI_Subtraction(struct __UI_t *A,struct __UI_t *B){
	/*
	* Esta função retorna a subtração de 'A' com 'B' (A-B).
	*
	* Em caso de erros, retorna NULL.
	*/
	if(A==NULL || A->Number==NULL || B==NULL || B->Number==NULL)	return NULL;
	struct __UI_t *AUI,*BUI,*NewUI;
	if(A->Negative==false && B->Negative==true){ // A+B
		BUI=UI_Abs(B);
		NewUI=UI_Sum(A,BUI);
		UI_Destroy(BUI);
		return NewUI;
	}
	if(A->Negative==true && B->Negative==false){ // -(A+B)
		AUI=UI_Abs(A);
		NewUI=UI_Sum(AUI,B);
		NewUI->Negative=true;
		UI_Destroy(AUI);
		return NewUI;
	}
	if(A->Negative==true && B->Negative==true){ // B-A (!= A-B)
		AUI=UI_Abs(A);
		BUI=UI_Abs(B);
		NewUI=UI_Subtraction(BUI,AUI);
		UI_Destroy(AUI);
		UI_Destroy(BUI);
		return NewUI;
	}
	char R=0,RA,RB,Sub,ABCmp;
	int i,ALen,BLen,Len;
	NewUI=(struct __UI_t *)malloc(sizeof(struct __UI_t));
	NewUI->Number=L_New();
	NewUI->Negative=false;
	ABCmp=UI_Cmp(A,B);
	if(ABCmp==0)	return NewUI;
	if(ABCmp<0){ // A-B
		AUI=A;
		BUI=B;
	}else{ // -(B-A)
		AUI=B;
		BUI=A;
		NewUI->Negative=true;
	}
	ALen=L_Size(AUI->Number);
	BLen=L_Size(BUI->Number);
	if(ALen<=BLen)	Len=ALen;
	else	Len=BLen;
	for(i=0;i<Len;i++){
		RA=L_GetAt(i,AUI->Number)-R;
		RB=L_GetAt(i,BUI->Number);
		if(RA<0 && RB<=0){
			R=1;
			Sub=9;
		}else if(RA<0){
			R=1;
			Sub=9-RB;
		}else if(RA<RB){
			R=1;
			Sub=10+RA-RB;
		}else{
			R=0;
			Sub=RA-RB;
		}
		L_Add(Sub,NewUI->Number);
	}
	if(ALen<BLen){
		for(i=ALen;i<BLen;i++){
			Sub=L_GetAt(i,BUI->Number)-R;
			if(Sub<0){
				R=1;
				L_Add(9,NewUI->Number);
			}else{
				R=0;
				L_Add(Sub,NewUI->Number);
			}
		}
	}else if(ALen>BLen){
		for(i=BLen;i<ALen;i++){
			Sub=L_GetAt(i,AUI->Number)-R;
			if(Sub<0){
				R=1;
				L_Add(9,NewUI->Number);
			}else{
				R=0;
				L_Add(Sub,NewUI->Number);
			}
		}
	}
	for(i=L_Size(NewUI->Number)-1;i>=0;i--){
		if(L_Get(NewUI->Number)==0)	L_Remove(NewUI->Number);
		else	break;
	}
	return NewUI;
}

struct __UI_t *UI_Product(struct __UI_t *A,struct __UI_t *B){
	/*
	* Esta função vai retornar o produto de 'A' com 'B' (A*B).
	*
	* Em caso de erros, retorna NULL.
	*/
	if(A==NULL || A->Number==NULL || B==NULL || B->Number==NULL)	return NULL;
	if(L_Size(A->Number)<=0)	return UI_NewFromInt(0);
	if(L_Size(B->Number)<=0)	return UI_NewFromInt(0);
	struct __UI_t *ProUI,*RUI,*AUI,*BUI,*NewUI;
	int i,j;
	char R;
	AUI=UI_Abs(A);
	BUI=UI_Abs(B);
	NewUI=UI_NewFromInt(0);
	for(i=0;i<L_Size(BUI->Number);i++){
		ProUI=UI_NewFromInt(0);
		R=0;
		if(L_GetAt(i,BUI->Number)!=0){
			for(j=0;j<i;j++)	L_Add(0,ProUI->Number);
			for(j=0;j<L_Size(AUI->Number);j++){
				R+=L_GetAt(i,BUI->Number)*L_GetAt(j,AUI->Number);
				L_Add(R%10,ProUI->Number);
				R/=10;
			}
			while(R>0){
				L_Add(R%10,ProUI->Number);
				R/=10;
			}
		}
		RUI=NewUI;
		NewUI=UI_Sum(RUI,ProUI);
		UI_Destroy(RUI);
		UI_Destroy(ProUI);
	}
	UI_Destroy(AUI);
	UI_Destroy(BUI);
	if(A->Negative!=B->Negative)	NewUI->Negative=true;
	else	NewUI->Negative=false;
	return NewUI;
}

struct __UI_t *UI_Quotient(struct __UI_t *A,struct __UI_t *B){
	/*
	* Esta função retorna o quociente de 'A' com 'B' (A/B).
	*
	* Em caso de erros, inclusive divisão por zero, retorna NULL.
	*/
	if(A==NULL || A->Number==NULL || B==NULL || B->Number==NULL)	return NULL;
	if(L_Size(B->Number)<=0)	return NULL;
	if(L_Size(A->Number)<=0)	return UI_NewFromInt(0);
	struct __UI_t *MinUI,*QuoUI,*RUI,*BUI,*NewUI;
	int i;
	char R;
	BUI=UI_Abs(B);
	MinUI=UI_NewFromInt(0);
	QuoUI=UI_NewFromInt(0);
	NewUI=UI_NewFromInt(0);
	for(i=L_Size(A->Number)-1;i>=0;i--){
		L_AddAt(L_GetAt(i,A->Number),0,QuoUI->Number);
		if(UI_Cmp(QuoUI,BUI)>0)	continue;
		R=-1;
		while(UI_Cmp(QuoUI,MinUI)<=0){
			RUI=QuoUI;
			QuoUI=UI_Subtraction(RUI,BUI);
			UI_Destroy(RUI);
			R++;
		}
		RUI=QuoUI;
		QuoUI=UI_Sum(QuoUI,BUI);
		UI_Destroy(RUI);
		L_AddAt(R,0,NewUI->Number);
	}
	UI_Destroy(BUI);
	UI_Destroy(MinUI);
	UI_Destroy(QuoUI);
	if(A->Negative!=B->Negative)	NewUI->Negative=true;
	else	NewUI->Negative=false;
	return NewUI;
}

char UI_Destroy(struct __UI_t *N){
	/*
	* Esta função realiza a limpeza de 'N' da memória.
	*
	* Ela retorna -1 em caso de erros, 0 caso parte da estrutura já esteja limpa, e 1 caso a limpeza tenha sido realizada com sucesso e por completo.
	* Nota 1: Esta função é constantemente usada anteriormente, pois ela auxilia na limpeza de "flags" da memória.
	*/
	if(N==NULL)	return -1;
	if(N->Number==NULL){
		free(N);
		return 0;
	}
	L_Destroy(N->Number);
	free(N);
	return 1;
}


