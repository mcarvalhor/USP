#include <stdio.h>
#include <stdlib.h>
#include <Image.h>



/*
 *      ==     IMAGENS P&B     ==
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
	IMG *A,*B,*Sum; // Flags para uso no código.
	int i,N;
	scanf("%d%*c",&N); // Ler número de operações.
	for(i=0;i<N;i++){ // Para cada operação...
		A=New_Img(); // Alocar imagem A.
		B=New_Img(); // Alocar imagem B.
		Read_Img(stdin,A); getchar(); // Ler A.
		Read_Img(stdin,B); getchar(); // Ler B.
		Sum=Join_Img(A,B); // Realizar a soma entre as duas.
		Destroy_Img(A); // Destruir A.
		Destroy_Img(B); // Destruir B.
		printf("%d %d pixels pretos.\n",i+1,Count_Img_Filled_Pixels(1024,Sum)/1024); // Imprimir o número de pixels/1024.
		Destroy_Img(Sum); // Destruir a soma.
	}
	return EXIT_SUCCESS;
}


