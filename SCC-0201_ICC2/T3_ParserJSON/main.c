#include <stdlib.h>
#include <stdio.h>
#include <json.h>



/*
 *	~      JSON Parser      ~
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
	int Line,ObjectN,ArrayN,PairN,StringN,NumberN,TrueN,FalseN,NullN;
	char *JSON_Line=NULL;
	size_t N;
	N=Line=ObjectN=ArrayN=PairN=StringN=NumberN=TrueN=FalseN=NullN=0; // Zerar todas as contagens.
	while( getline(&JSON_Line,&N,stdin)!=-1 ){ // Se tiver uma linha pra ser lida...
		Line++; // Incrementar o número da linha onde estamos.
		if(JSON_Check(JSON_Line,&ObjectN,&ArrayN,&PairN,&StringN,&NumberN,&TrueN,&FalseN,&NullN)==0){ // Essa linha obedece a gramática 'G'?
			printf("Error line %d\n",Line); // Não. "Throw exception".
			break; // Parar a leitura já.
		}
	}
	printf("Number of Objects: %d\n",ObjectN);
	printf("Number of Arrays: %d\n",ArrayN);
	printf("Number of Pairs: %d\n",PairN);
	printf("Number of Strings: %d\n",StringN);
	printf("Number of Numbers: %d\n",NumberN);
	printf("Number of Trues: %d\n",TrueN);
	printf("Number of Falses: %d\n",FalseN);
	printf("Number of Nulls: %d\n",NullN);
	free(JSON_Line); // Liberar a nossa string de expressão JSON da memória.
	return EXIT_SUCCESS;
}



