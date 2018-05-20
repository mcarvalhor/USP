#include <stdio.h>
#include <stdlib.h>
#include <TreeBinSearch.h>



/*
 *      ==         ABB         ==
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



int main(int argc, char **argv){
	BIN_SEARCH_TREE *Main_Tree;
	char R;
	int S;
	Main_Tree=Create_Tree(); // Criar árvore.
	while( (R=getchar())!='X' ) {
		switch(R){
			case 'I': // Inserir um valor.
				scanf("%d",&S);
				if(Find_In(S,Main_Tree)==1)	printf("Chave existente\n"); // Impedir multiplas inserções de um mesmo elemento.
				else	Insert_Into(S,Main_Tree);
			break;
			case 'D': // Remover um valor.
				scanf("%d",&S);
				if(Remove_First_From(S,Main_Tree)==1)	printf("%d\n",S);
				else	printf("Nao encontrado\n");
			break;
			case 'B': // Buscar um valor.
				scanf("%d",&S);
				if(Find_In(S,Main_Tree)==1)	printf("Encontrado\n");
				else	printf("Nao encontrado\n");
			break;
			case 'Y': // Imprimir de todas as formas (cascata).
			case 'N': // Imprimir em ordem.
				Print_InOrder(stdout,Main_Tree);
				if(R=='N')	break;
			case 'E': // Imprimir pré-ordem.
				Print_PreOrder(stdout,Main_Tree);
				if(R=='E')	break;
			case 'O': // Imprimir pós-ordem.
				Print_PostOrder(stdout,Main_Tree);
				if(R=='O')	break;
			default: // Imprimir por largura.
				Print_LevelOrder(stdout,Main_Tree);
		}
		getchar(); // Eliminar pulo de linha.
	}
	Destroy_Tree(Main_Tree);
	return EXIT_SUCCESS;
}


