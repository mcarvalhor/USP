#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <super_file.h>
#include "stack.h"



/*
 *	~          JSON         ~
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



// GetTrue ontém de 'Fl' um tipo "true".
#define GetTrue(Str,St,Fl) { \
	fscanf(Fl,"%4s",Str); \
	if(strcmp(Str,"true")!=0){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
}

// GetFalse obtém de 'Fl' um tipo "false".
#define GetFalse(Str,St,Fl) { \
	fscanf(Fl,"%5s",Str); \
	if(strcmp(Str,"false")!=0){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
}

// GetNull obtém de 'Fl' um tipo "null".
#define GetNull(Str,St,Fl) { \
	fscanf(Fl,"%4s",Str); \
	if(strcmp(Str,"null")!=0){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
}

// GetString obtém de 'Fl' uma string formatada de acordo com a especificação.
#define GetString(Ch,St,Fl) { \
	if(getc(Fl)!='"'){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
	while( (Ch=getc(Fl))!=EOF && Ch!='"' ) { \
		if(Ch=='\\' && ( (Ch=getc(Fl))=='"' || Ch=='b' || Ch=='f' || Ch=='n' || Ch=='r' || Ch=='t' || Ch=='u' || Ch=='\\' ) ){ \
			S_Destroy(St); \
			fclose(Fl); \
			return 0; \
		} \
	} \
	if(Ch==EOF || Ch=='\\'){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
}

// GetNumber obtém de 'Fl' um número, seja ele real ou inteiro.
#define GetNumber(Ch,Aux,St,Fl) { \
	if( (Ch=SF_PreviewChar(Fl))=='+' || Ch=='-')	getc(FStream); \
	Aux=0; \
	while( (Ch=SF_PreviewChar(Fl))!=EOF && Ch>='0' && Ch<='9') { getc(Fl); Aux++; } \
	if(Aux<1){ \
		S_Destroy(St); \
		fclose(Fl); \
		return 0; \
	} \
	if(SF_PreviewChar(Fl)=='.'){ \
		getc(Fl); \
		Aux=0; \
		while( (Ch=SF_PreviewChar(Fl))!=EOF && Ch>='0' && Ch<='9') { getc(Fl); Aux++; } \
		if(Aux<1){ \
			S_Destroy(St); \
			fclose(Fl); \
			return 0; \
		} \
	} \
	if( (Ch=SF_PreviewChar(Fl))=='e' || Ch=='E' ){ \
		getc(Fl); \
		if( (Ch=SF_PreviewChar(Fl))=='+' || Ch=='-')	getc(Fl); \
		Aux=0; \
		while( (Ch=SF_PreviewChar(Fl))!=EOF && Ch>='0' && Ch<='9') { getc(Fl); Aux++; } \
		if(Aux<1){ \
			S_Destroy(St); \
			fclose(Fl); \
			return 0; \
		} \
	} \
}



enum __json_set_types_t { Set_Comma=1, Set_Array, Set_Object }; // Representa os tipos possíveis de um empilhamento.



char JSON_Check(char *JSON_Expression,int *ObjectN,int *ArrayN,int *PairN,int *StringN,int *NumberN,int *TrueN,int *FalseN,int *NullN){
	/*
	* Esta função verifica se 'JSON_Expression' segue a gramática "G" dada por um "Value" de JSON.
	* NOTA:	EU NÃO USEI REGEX.
	*
	* Ela retorna 0 se 'JSON_Expression' não concorda com a gramática, 1 se concorda, e -1 em caso de erros na verificação.
	*/
	if(JSON_Expression==NULL)	return -1;
	STACK *Set=S_New();
	FILE *FStream;
	char R,S[10];
	int T;
	FStream=fmemopen(JSON_Expression,strlen(JSON_Expression),"r"); // Abrir stream a partir da string.
	do { // Ler todos os 'Values' da string...
		SF_ReadWhites(FStream); // Esta função é chamada frequentemente para ignorar espaçamentos entre os elementos.
		R=SF_PreviewChar(FStream);
		if(S_Size(Set)>1 && S_Get(Set)==Set_Comma){ // Esperando uma vírgula porque é um novo par para um "Array" ou "Object", ou desempilhamento.
			S_Pop(Set);
			if(R==','){
				getc(FStream); // Consumir vírgula.
				if(S_Get(Set)==Set_Object){
					(*PairN)++;
					SF_ReadWhites(FStream);
					GetString(R,Set,FStream); // Ler a "key" do par.
					(*StringN)++;
					SF_ReadWhites(FStream);
					if(getc(FStream)!=':'){ // Não é um par "key-value" válido.
						S_Destroy(Set);
						fclose(FStream);
						return 0;
					}
				}
			}else if(R=='}'){
				(*PairN)++; // Fechamos um objeto. Incrementar o valor correspondente ao último par.
			}else if(R!=']'){
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			continue;
		}else if(R=='{'){ // Só pode ser um "Object".
			getc(FStream);
			S_Push(Set_Object,Set); // Empilhar.
			SF_ReadWhites(FStream);
			if(SF_PreviewChar(FStream)=='}')	continue; // Caso em que o "Object" não possui nada dentro.
			GetString(R,Set,FStream); // Ler "key" do primeiro par.
			(*StringN)++;
			SF_ReadWhites(FStream);
			if(getc(FStream)!=':'){ // Não é um par "key-value" válido.
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			continue;
		}else if(R=='}'){ // Só pode ser o fim de um "Object".
			getc(FStream);
			if(S_Size(Set)>0 && S_Get(Set)==Set_Object){
				S_Pop(Set); // Desempilhar
			}else{ // Esse "}" não é esperado aqui.
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*ObjectN)++;
		}else if(R=='['){ // Só pode ser um "Array".
			getc(FStream);
			S_Push(Set_Array,Set); // Empilhar.
			continue;
		}else if(R==']'){ // Só pode ser o fim de um "Array".
			getc(FStream);
			if(S_Size(Set)>0 && S_Get(Set)==Set_Array){
				S_Pop(Set); // Desempilhar.
			}else{ // Esse "]" não é esperado aqui.
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*ArrayN)++;
		}else if(R=='t'){ // Só pode ser "true".
			GetTrue(S,Set,FStream);
			if( (R=SF_PreviewChar(FStream))!=EOF && R!=' ' && R!=',' && R!=']' && R!='}' ){ // Verificar integridade do "true.
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*TrueN)++;
		}else if(R=='f'){ // Só pode ser "false".
			GetFalse(S,Set,FStream);
			if( (R=SF_PreviewChar(FStream))!=EOF && R!=' ' && R!=',' && R!=']' && R!='}' ){ // Verificar integridade do "false".
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*FalseN)++;
		}else if(R=='n'){ // Só pode ser "null".
			GetNull(S,Set,FStream);
			if( (R=SF_PreviewChar(FStream))!=EOF && R!=' ' && R!=',' && R!=']' && R!='}' ){ // Verificar integridade do "null".
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*NullN)++;
		}else if(R=='"'){ // Só poder ser uma "String".
			GetString(R,Set,FStream);
			if( (R=SF_PreviewChar(FStream))=='"' ){ // Verificar integridade da "String".
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*StringN)++;
		}else if(R=='+' || R=='-' || (R>='0' && R<='9')){ // Só pode ser um "Number".
			GetNumber(R,T,Set,FStream);
			if( (R=SF_PreviewChar(FStream))!=EOF && R!=' ' && R!=',' && R!=']' && R!='}' ){ // Verificar integridade do "Number".
				S_Destroy(Set);
				fclose(FStream);
				return 0;
			}
			(*NumberN)++;
		}else if(R==','){
			getc(FStream); // Consumir vírgulas perdidas por aí. (Atenção: isso não é aceito oficialmente pela gramática do JSON, mas o PDF do trabalho permite).
			continue;
		}else{ // Algum caractere não identificado apareceu. Não é uma expressão válida.
			S_Destroy(Set);
			fclose(FStream);
			return 0;
		}
		if(S_Size(Set)>0)	S_Push(Set_Comma,Set); // Caso em que esperamos uma vírgula e um novo elemento.
	} while( S_Size(Set)>0 );
	S_Destroy(Set);
	SF_ReadWhites(FStream);
	if(getc(FStream)!=EOF){ // Há caracteres após o "Value" do JSON que não são válidos?
		fclose(FStream); // Sim. Expressão inválida.
		return 0;
	}
	fclose(FStream);
	return 1; // Passou por todos os testes e é válido.
}



