#include <stdlib.h>
#include <stdio.h>
#include <calc.h>
#include <super_file.h>



/*
 *	~  Calc. de Expressões  ~
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
	LIST *Calc;
	char R;
	double S;
	int i=0;
	while(!feof(stdin)){
		if(i)	printf("\n"); /* Tratamento para não imprimir \n ao fim do último item. */
		if( (Calc=NewExpression(stdin))!=NULL ){ /* É uma expressão válida? */
			if(CalcExpression(&S,Calc)!=1) /* Sim. Agora, possui alguma divisão por zero? */
				printf("Expressao incorreta"); /* Sim. Não é válido. */
			else
				printf("%.02f",S); /* Não. Imprimir solução da expressão. */
			DestroyExpression(Calc);
		}else{
			while( (R=SF_PreviewChar(stdin))!=',' && R!=';' && R!=EOF )	getchar(); /* Deu erro, vamos consumir a entrada até o início da próxima expressão. */
			printf("Expressao incorreta"); /* Não. Imprimir que é uma expressão inválida. */
		}
		if(getchar()==',')	break;
		i++;
	}
	return EXIT_SUCCESS;
}



