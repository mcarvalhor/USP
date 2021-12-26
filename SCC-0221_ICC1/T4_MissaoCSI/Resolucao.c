#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
	Matheus Carvalho Raimundo
	mcarvalhor@usp.br
*/


// Definir uma estrutura capaz de lidar com nossas necessidades e armazenar todos os dados lidos da imagem.
typedef struct __ImageData {
	int X,Y;
	char Type;
	unsigned char Max;
	unsigned char Min;
	unsigned char *Pixels;
} IMG;


// Declarar quais funções existem em nosso código.
char LoadImage(char *FilePath,IMG *ImgData);
void ProcessImage(char Procedure,IMG *ImgData);
void ProcessImage__FloodFill(int pixelX,int pixelY,IMG *ImgData);
void OutputImage(IMG *ImgData);
void DestroyImage(IMG *ImgData);


int main(){
	IMG Carro;
	char Procedure, FileName[100];
	Procedure=getchar(); // Obter o procedimento (1, 2 ou 3).
	fgets(FileName,100,stdin); // Liberar '\n' do buffer.
	fgets(FileName,100,stdin); // Ler nome do arquivo.
	if(FileName[strlen(FileName)-1]=='\n'){
		FileName[strlen(FileName)-1]='\0';
	}
	if(LoadImage(FileName,&Carro)){ // A imagem existe e pode ser lida?
		ProcessImage(Procedure,&Carro); // Sim. Processa ela, retorna o resultado e limpa a memória.
		OutputImage(&Carro);
		DestroyImage(&Carro);
	}else{
		printf("> Erro: arquivo especificado nao existe ou nao pode ser lido.\n"); // Não. Notifique o usuário.
	}
	return EXIT_SUCCESS;
}



char LoadImage(char *FilePath,IMG *ImgData){
	/*
	 * Esta função faz a leitura do arquivo de imagem e salva todos os dados dela na estrutura 'ImgData'.
	 * Retorna 1 se a leitura ocorrer com sucesso ou 0 em caso de erros.
	 */
	int i,R;
	char Line[100];
	FILE *FStream=fopen(FilePath,"rb"); // Abrir arquivo de forma leitura binária (se for o caso).
	if(FStream){ // Foi aberto com sucesso?
		fgets(Line,100,FStream); // Sim. Obtenha o tipo de arquivo (ASCII ou Binário).
		ImgData->Type=Line[1];
		fgets(Line,100,FStream); // Leia e ignore o comentário.
		fscanf(FStream,"%d %d",&ImgData->X,&ImgData->Y); // Ler as dimensões da imagem.
		fscanf(FStream,"%d",&R); getc(FStream); // Ler e ignorar mais uma linha.
		ImgData->Min=255;
		ImgData->Max=0;
		ImgData->Pixels=(unsigned char*)malloc(sizeof(unsigned char)*ImgData->X*ImgData->Y);
		if(ImgData->Type=='2'){ // O arquivo é do tipo ASCII?
			for(i=0;i<ImgData->X*ImgData->Y;i++){ // Sim, então para cada pixel...
				fscanf(FStream,"%d",&R); // Leia um inteiro do arquivo que representa esse pixel e salva no nosso vetor de pixels.
				ImgData->Pixels[i]=(unsigned char)R;
				if(R>ImgData->Max){
					ImgData->Max=R;
				}
				if(R<ImgData->Min){
					ImgData->Min=R;
				}
			}
		}else{
			for(i=0;i<ImgData->X*ImgData->Y;i++){ // Não, então só pode ser do tipo Binário pela especificação. Para cada pixel...
				fread(&ImgData->Pixels[i],sizeof(unsigned char),1,FStream); // Leia um byte do arquivo que representa esse pixel e salva no nosso vetor de pixels.
				if(ImgData->Pixels[i]>ImgData->Max){
					ImgData->Max=ImgData->Pixels[i];
				}
				if(ImgData->Pixels[i]<ImgData->Min){
					ImgData->Min=ImgData->Pixels[i];
				}
			}
		}
		fclose(FStream);
		return 1;
	}
	return 0;
}


void ProcessImage(char Procedure,IMG *ImgData){
	/*
	 * Esta função faz as 3 operações possíveis na imagem.
	 */
	int i;
	double k;
	if(Procedure=='1'){ // Fazer logarítmo.
		k=255/(double)log10(1+ImgData->Max);
		for(i=0;i<ImgData->X*ImgData->Y;i++){
			ImgData->Pixels[i]=(unsigned char)floor(k*log10(1+ImgData->Pixels[i])); // Aplica a equação padrão (logarítmo) para cada pixel.
		}
	}else if(Procedure=='2'){ // Fazer expansão de contraste.
		k=255/(double)(ImgData->Max-ImgData->Min);
		for(i=0;i<ImgData->X*ImgData->Y;i++){
			ImgData->Pixels[i]=(unsigned char)floor(k*(ImgData->Pixels[i]-ImgData->Min)); // Aplica a equação padrão (razão e proporção) para cada pixel.
		}
	}else{ // Fazer o "Flood Fill" (Baldinho do PaintBrush).
		ProcessImage__FloodFill(ImgData->X/2,ImgData->Y/2,ImgData); // Chamar função separada que faz tal operação, iniciando no centro da imagem.
	}
}


void ProcessImage__FloodFill(int pixelX,int pixelY,IMG *ImgData){
	/*
	 * Esta função é responsável por realizar a terceira operação da imagem (FloodFill ou Baldinho do PaintBrush).
	 */
	if( (pixelX>=0) && (pixelY>=0) && (pixelX<ImgData->X) && (pixelY<ImgData->Y) &&
			(ImgData->Pixels[pixelY*ImgData->X+pixelX]!=100) &&
			(ImgData->Pixels[pixelY*ImgData->X+pixelX]!=255) ){ // A operação de 'FloodFill' pode ser realizada?
		ImgData->Pixels[pixelY*ImgData->X+pixelX]=100; // Sim. Tornar este pixel cinza (100).
		ProcessImage__FloodFill(pixelX-1,pixelY,ImgData); // Um pixel para a esquerda.
		ProcessImage__FloodFill(pixelX,pixelY-1,ImgData); // Um pixel para cima.
		ProcessImage__FloodFill(pixelX+1,pixelY,ImgData); // Um pixel para a direita.
		ProcessImage__FloodFill(pixelX,pixelY+1,ImgData); // Um pixel para baixo.
	}
}


void OutputImage(IMG *ImgData){
	/*
	 * Esta função imprime a imagem em 'stdout'.
	 */
	int i;
	printf("P2\n# CREATOR: Image Generator SCC-221 - ICC I\n");
	printf("%d %d\n255\n",ImgData->X,ImgData->Y);
	for(i=0;i<ImgData->X*ImgData->Y;i++){
		printf("%d\n",ImgData->Pixels[i]);
	}
}


void DestroyImage(IMG *ImgData){
	/*
	 * Esta função limpa a memória alocada dinamicamente.
	 */
	free(ImgData->Pixels);
}
