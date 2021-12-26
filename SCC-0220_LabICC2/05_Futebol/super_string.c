#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>



/*
 *	~      SUPER STRING     ~
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
// Esta biblioteca está em fase de desenvolvimento.



char *SS_ReadUntil(char *String,char Ch,FILE *FStream){
	/*
	* Esta função lê de 'FStream' uma string até chegar em um caractere 'Ch'. Ela não inclui 'Ch' na string gerada, mas inclui o caracter NULL "\0".
	* 	- Se 'String' for igual a NULL, a função vai alocar na memória uma string e retornar o endereço para tal.
	* 	- Se 'String' for diferente de NULL, a função vai modificar a própria 'String'.
	*	  CUIDADO: Nesse caso, a função pode e vai sobrescrever indefinidamente 'String', podendo inclusive ultrapassar os limites de tal, ocasionando um erro de segmentação (Out of Bounds).
	*
	* Ela retorna um ponteiro para a string alocada ou modificada.
	*/
	int n=0;
	char R,*Aux=String;
	if(Aux==NULL){
		Aux=malloc(sizeof(char)*1);
		while( (R=getc(FStream))!=EOF && R!=Ch){
			Aux=(char *)realloc(Aux,sizeof(char)*n+2);
			Aux[n++]=R;
		}
	}else
		while( (R=getc(FStream))!=EOF && R!=Ch)	Aux[n++]=R;
	Aux[n]='\0';
	if(R!=EOF)	fseek(FStream,-1,SEEK_CUR);
	return Aux;
}

int SS_ReadAllWhites(FILE *FStream){
	/*
	* Esta função avança o ponteiro de 'FStream' até um ponto em que não existam mais caracteres considerados "white-space".
	*
	* Ela retorna o número de caracteres pulados para chegar até tal, ou -1 em caso de erros.
	*/
	char R;
	int n=0;
	while( (R=getc(FStream))!=EOF && isspace(R) )	n++;
	if(R==EOF)	return -1;
	fseek(FStream,-1,SEEK_CUR);
	return n;
}

char SS_InsensitiveCmp(char *A,char *B){
	/*
	* Esta função compara 'A' com 'B' em formato "case-insensitive", ou seja, sem levar em consideração a diferença entre maiúsculos e minúsculos.
	*
	* Ela retorna -1 para 'A' maior que 'B', 0 se forem iguais, e 1 para 'B' maior que 'A'.
	*/
	int i,NA=strlen(A),NB=strlen(B);
	char *StrA=(char *)malloc(sizeof(char)*NA),*StrB=(char *)malloc(sizeof(char)*NB);
	for(i=0;i<NA;i++)	StrA[i]=tolower(A[i]);
	for(i=0;i<NB;i++)	StrB[i]=tolower(B[i]);
	i=strcmp(StrA,StrB);
	free(StrA);
	free(StrB);
	if(i>0)	return 1;
	else if(i<0)	return -1;
	else	return 0;
}



