#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <skiplist.h>



/*
 *	~       Dicionário      ~
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



int main(int argc, char **argv){
	char Operacao[15], Word[51], Meaning[141];
	SKIPLIST *Words;
	Words=New_Dictionary(time(NULL));
	while(scanf("%s",Operacao)==1) /* Enquanto existirem operações para serem executadas... */
		if(!strcmp(Operacao,"insercao")){
			scanf("%50s %140[^\n^\r]s",Word,Meaning);
			if(Insert_Word(Word,Meaning,Words)!=1)	printf("OPERACAO INVALIDA\n"); /* Inserir ou retornar mensagem de erro. */
		}else if(!strcmp(Operacao,"alteracao")){
			scanf("%50s %140[^\n^\r]s",Word,Meaning);
			if(Set_Word(Word,Meaning,Words)!=1)	printf("OPERACAO INVALIDA\n"); /* Alterar ou retornar mensagem de erro. */
		}else if(!strcmp(Operacao,"remocao")){
			scanf("%50s",Word);
			if(Remove_Word(Word,Words)!=1)	printf("OPERACAO INVALIDA\n"); /* Remover ou retornar mensagem de erro. */
		}else if(!strcmp(Operacao,"busca")){
			scanf("%50s",Word);
			if(Print_Word(Word, stdout, Words)!=1)	printf("OPERACAO INVALIDA\n"); /* Imprimir definição ou retornar mensagem de erro. */
		}else if(!strcmp(Operacao,"impressao")){
			scanf("%50s",Word);
			if(Print_Words_Starting_With(Word[0],stdout,Words)!=1)	printf("NAO HA PALAVRAS INICIADAS POR %c\n", Word[0]); /* Imprimir definições ou retornar mensagem de aviso. */
		}else printf("OPERACAO INVALIDA\n");
	Destroy_Dictionary(Words);
	return EXIT_SUCCESS;
}