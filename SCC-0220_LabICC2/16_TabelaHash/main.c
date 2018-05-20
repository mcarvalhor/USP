#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <HashTree.h>



/*
 *	~      Tabela Hash      ~
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



#define MAX_M 100



int main(int argc, char **argv){
	HASHTREE *BigTable=HTree_New(MAX_M);
	char Operation[10];
	int Aux;

	scanf("%5s", Operation);
	while(Operation[0]!='f'){
		switch(Operation[0]){
			case 'i':
				scanf("%d", &Aux);
				HTree_Insert(Aux, BigTable);
			break;
			case 'b':
				scanf("%d", &Aux);
				if(HTree_Count(Aux, BigTable) > 0)	printf("encontrado\n");
				else	printf("nao encontrado\n");
			break;
			case 'r':
				scanf("%d", &Aux);
				HTree_Remove(Aux, BigTable);
			break;
			default:
				HTree_Traverse(stdout, BigTable);
		}
		scanf("%5s", Operation);
	}

	HTree_Destroy(BigTable);

	return EXIT_SUCCESS;
}


