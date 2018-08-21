#include <stdio.h>
#include <stdlib.h>
#include "UnlimitedInteger.h"


/*
 *      ==   NÚMEROS IMENSOS   ==
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
	char Function[50];
	UI *A,*B,*R=NULL;
	fgets(Function,sizeof(Function),stdin);
	A=UI_Read(stdin);
	B=UI_Read(stdin);
	if(Function[2]=='M'){ // SOMA
		R=UI_Sum(A,B);
		UI_Write(stdout,R);
	}else if(Function[2]=='B'){ // SUBTRACAO
		R=UI_Subtraction(A,B);
		UI_Write(stdout,R);
	}else if(Function[2]=='L'){ // MULTIPLICACAO
		R=UI_Product(A,B);
		UI_Write(stdout,R);
	}else if(Function[2]=='V'){ // DIVISAO
		R=UI_Quotient(A,B);
		UI_Write(stdout,R);
	}else{
		printf("Operação não reconhecida.");
	}
	printf("\n");
	UI_Destroy(A);
	UI_Destroy(B);
	UI_Destroy(R);
	return EXIT_SUCCESS;
}


