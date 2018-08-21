#include <stdlib.h>
#include <stdio.h>



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



enum __colour_t { empty = 0, full }; // Preto ou Branco

struct __img_t { // Estrutura da imagem
	enum __colour_t Colour; // Cor
	struct __img_t *Children[4]; // Quadrantes filhos
};



struct __img_t *New_Img(){
	/*
	* Esta função aloca na memória uma nova imagem e retorna um ponteiro para esta.
	*/
	struct __img_t *Aux=(struct __img_t *)malloc(sizeof(struct __img_t));
	int i;
	Aux->Colour=empty;
	for(i=0;i<4;i++)
		Aux->Children[i]=NULL;
	return Aux;
}

char Read_Img(FILE *FStream,struct __img_t *Img){
	/*
	* Esta função realiza a leitura de 'Fstream' de uma String que represente uma imagem e salva em 'Img'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(Img==NULL)	return 0;
	char R=getc(FStream);
	int i,j;
	if(R=='p')
		for(i=0;i<4;i++){
			Img->Children[i]=(struct __img_t *)malloc(sizeof(struct __img_t));
			Img->Children[i]->Colour=empty;
			for(j=0;j<4;j++)
				Img->Children[i]->Children[j]=NULL;
			Read_Img(FStream,Img->Children[i]);
		}
	else if(R=='f')	Img->Colour=full;
	else	Img->Colour=empty;
	return 1;
}

struct __img_t *Join_Img(struct __img_t *A, struct __img_t *B){
	/*
	* Esta função realiza a soma entre duas imagens 'A' e 'B', retornando a imagem resultante.
	*
	* Ela retorna um ponteiro para a imagem resultante, ou NULL em caso de erros.
	*/
	if(A==NULL && B==NULL)	return NULL;
	struct __img_t *Aux=New_Img();
	int i;
	if(A==NULL){
		if(B->Colour==full)	Aux->Colour=full;
	}else if(B==NULL){
		if(A->Colour==full)	Aux->Colour=full;
	}else if(A->Colour==full || B->Colour==full){
		Aux->Colour=full;
	}else{
		for(i=0;i<4;i++)
			Aux->Children[i]=Join_Img(A->Children[i],B->Children[i]);
	}
	return Aux;
}

int Count_Img_Filled_Pixels(int Resolution,struct __img_t *Img){
	/*
	* Esta função conta exatamente o número de pixels pretos em 'Img', considerando uma resolução de imagem de 'Resolution'.
	*
	* Ela retorna o número de pixels pretos, ou -1 em caso de erros.
	*/
	if(Img==NULL)	return -1;
	int i,R,Sum=0;
	if(Img->Colour==full)	Sum+=Resolution*Resolution;
	else
		for(i=0;i<4;i++){
			R=Count_Img_Filled_Pixels(Resolution/2,Img->Children[i]);
			if(R>0)	Sum+=R;
		}
	return Sum;
}

char Destroy_Img(struct __img_t *Img){
	/*
	* Esta função remove da memória a imagem 'Img' e todos seus filhos.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(Img==NULL)	return 0;
	int i;
	for(i=0;i<4;i++)
		Destroy_Img(Img->Children[i]);
	free(Img);
	return 1;
}


