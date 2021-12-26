#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



/*
 *	~      SUPER FILE       ~
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
/*
* Esta biblioteca adiciona algumas funções a leitura de Streams.
* Ainda está em fase de desenvolvimento.
*/



char SF_PreviewChar(FILE *FStream){ /* Release */
	/*
	* Essa função lê o próximo caractere de 'FStream' e retorna uma posição em 'FStream', retornando o próprio caractere lido.
	* Em suma, ela lê um caractere de 'FStream' sem consumir ele.
	*
	* Ela retorna o próximo caractere de 'FStream', ou EOF em caso de erros.
	*/
	char R=getc(FStream);
	fseek(FStream,-1,SEEK_CUR);
	return R;
}

int SF_ReadWhites(FILE *FStream){ /* Release */
	/*
	* Esta função vai ler caracteres de 'FStream' até chegar em algo que não é considerado "white-space". Após isso, ela retorna uma posição em 'FStream'.
	* Em suma, ela ignora todos os caracteres "white-space" de 'Fstream'.
	*
	* Ela retorna o número de caracteres lidos, ou -1 em caso de erros.
	*/
	char R;
	int n=0;
	while( (R=getc(FStream))!=EOF && isspace(R)!=0 )	n++;
	if(R!=EOF)	fseek(FStream,-1,SEEK_CUR);
	return n;
}

int SF_ReadUntil(char Key,FILE *FStream){
	/*
	* Esta função vai ler caracteres de 'FStream' até chegar em 'Key'. Após isso, ela retorna uma posição em 'FStream'.
	* Em suma, ela ignora todos os caracteres diferentes de 'Key' de 'Fstream'.
	*
	* Ela retorna o número de caracteres lidos, ou -1 em caso de erros.
	*/
	char R;
	int n=0;
	while( (R=getc(FStream))!=EOF && R!=Key )	n++;
	if(R!=EOF)	fseek(FStream,-1,SEEK_CUR);
	return n;
}

char *SF_ReadString(FILE *FStream){
	/*
	* Esta função vai tentar obter uma string de 'FStream'.
	* Caracteres "white-space" que aparecerem antes de uma string serão ignorados.
	* Ela vai parar de ler após o primeiro caractere "white-space" aparecer, ou fim do arquivo.
	* Em suma, ela lê uma palavra de 'Fstream'.
	*
	* Ela retorna um ponteiro para uma string na memória, ou NULL em caso de erros.
	*/
	int n=0;
	char R,*ReadString=NULL;
	while( (R=getc(FStream))!=EOF && isspace(R)!=0 ) { }
	if(R==EOF)	return NULL; /* Chegou no fim do arquivo e nenhuma string foi encontrada. */
	ReadString=(char *)malloc(sizeof(char)*2);
	ReadString[n++]=R;
	while( (R=getc(FStream))!=EOF && isspace(R)==0 ) {
		ReadString=(char *)realloc(ReadString,sizeof(char)*(n+2));
		ReadString[n++]=R;
	}
	ReadString[n]='\0';
	if(R!=EOF)	fseek(FStream,-1,SEEK_CUR);
	return ReadString;
}

char *SF_ReadFormattedString(FILE *FStream){
	/*
	* Esta função vai tentar obter uma string de 'FStream'.
	* Caracteres que aparecerem antes de uma aspas duplas de abertura *"* serão ignorados.
	* Ela vai ler a string até encontrar uma aspas duplas *"* que completa as aspas de abertura.
	* A diferença dessa função com "ReadString" é que esta lê uma string pré-formatada, ou seja, pode inclusive ter caracteres como aspas duplas *\"* e espaços dentro da string.
	* Em suma, ela lê uma string de 'Fstream' dentro de aspas duplas *"*.
	*
	* Ela retorna um ponteiro para uma string na memória, ou NULL em caso de erros.
	*/
	int n=0;
	char R,*ReadString=NULL;
	while( (R=getc(FStream))!=EOF && R!='"' ) { }
	if(R==EOF)	return NULL; /* Chegou no fim do arquivo e nenhuma string foi encontrada. */
	ReadString=(char *)malloc(sizeof(char)*1);
	while( (R=getc(FStream))!=EOF && R!='"' ) {
		if(R=='\\'){
			R=getc(FStream);
			if(R==EOF)	break;
			else if(R=='n')	R='\n';
			else if(R=='t')	R='\t';
			else continue;
		}
		ReadString=(char *)realloc(ReadString,sizeof(char)*(n+2));
		ReadString[n++]=R;
	}
	ReadString[n]='\0';
	if(R==EOF)	return NULL; /* As aspas duplas *"* de fechamento não foram encontradas. */
	else	fseek(FStream,-1,SEEK_CUR);
	return ReadString;
}



