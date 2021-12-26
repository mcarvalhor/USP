#include <stdlib.h>
#include <stdio.h>
#include <redblacktree.h>



/*
 *	~      Rubro Negra      ~
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



int compare_integer(void *, void *);
void print_integer(void *);



int main(int argc, char **argv){
	int i, N, Operation, Aux;
	RBT *Tree=RBT_New(sizeof(int), compare_integer, NULL); /* Alocar rubro-negra */
	scanf("%d", &N);
	for(i=0; i<N; i++){
		scanf("%d", &Operation);
		switch(Operation){
			case 1: /* Inserção */
				scanf("%d", &Aux);
				RBT_Insert(&Aux, Tree);
			break;
			case 2: /* Sucessor */
				scanf("%d", &Aux);
				if(RBT_Successor(&Aux, &Aux, Tree)==1)	printf("%d\n", Aux);
				else	printf("erro\n");
			break;
			case 3: /* Predecessor */
				scanf("%d", &Aux);
				if(RBT_Predecessor(&Aux, &Aux, Tree)==1)	printf("%d\n", Aux);
				else	printf("erro\n");
			break;
			case 4: /* Máximo */
				if(RBT_Max(&Aux, Tree)==1)	printf("%d\n", Aux);
				else	printf("erro\n");
			break;
			case 5: /* Mínimo */
				if(RBT_Min(&Aux, Tree)==1)	printf("%d\n", Aux);
				else	printf("erro\n");
			break;
			case 6: /* Pré-ordem */
				RBT_PreOrder(print_integer, Tree);
				printf("\n");
			break;
			case 7: /* Em-ordem */
				RBT_InOrder(print_integer, Tree);
				printf("\n");
			break;
			case 8: /* Pós-ordem */
				RBT_PostOrder(print_integer, Tree);
				printf("\n");
			break;
			default: /* Operação inválida */
				printf("erro\n");
		}
	}
	RBT_Destroy(Tree); /* Destruir rubro-negra */
	return EXIT_SUCCESS;
}



int compare_integer(void *PA, void *PB){
	/*
	* Como criei um TAD genérico, essa função apenas compara dois inteiros.
	*/
	int *A=PA, *B=PB;
	return (*B)-(*A);
}

void print_integer(void *A){
	/*
	* Como criei um TAD genérico, essa função apenas imprime um inteiro na saída padrão.
	*/
	printf("%d ", *((int *)A));
}


