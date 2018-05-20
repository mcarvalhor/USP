#include <stdlib.h>
#include <stdio.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

/*
 * Meu programa baseia-se principalmente no conceito de Matriz (para representar o Labirinto) e recursao (para caminhar sob ele).
 *
 * A funcao principal main() vai chamar GenerateLabyrinth() para gerar o labirinto e chamar WalkTowards() para verificar o caminho a ser tomado ate o fim
 * do labirinto.
 *
 */

// Gerar um "pseudo bool" na linguagem.
#define bool char
#define true 1
#define false 0

// Declarar as funcoes que existem no codigo para o compilador.
void GenerateLabyrinth(char (*Lab)[7]);
bool WalkTowards(int O_L,int O_C,int L,int C,int *Solution, char (*Lab)[7]);

int main(){
	int i,start_point;
	char Labyrinth[7][7]; // O labirinto sera armazenado nessa variavel.
	int PathToSolution[21]={0}; // Vetor de solucao do labirinto. O primeiro elemento eh a contagem de passos que andamos.
	GenerateLabyrinth(Labyrinth); // Gerar labirinto.
	scanf(" %d",&start_point); // Por onde vamos comecar nesse labirinto?
	if(start_point/7==6){
		WalkTowards(6,start_point%7,5,start_point%7,PathToSolution,Labyrinth); // Comecamos por baixo do labirinto. Comeca a andar!
	}else if(start_point/7==0){
		WalkTowards(0,start_point%7,1,start_point%7,PathToSolution,Labyrinth); // Comecamos por cima...
	}else if(start_point%7==6){
		WalkTowards(start_point/7,6,start_point/7,5,PathToSolution,Labyrinth); // Comecamos pela direita...
	}else{
		WalkTowards(start_point/7,0,start_point/7,1,PathToSolution,Labyrinth); // Comecamos pela esquerda...
	}
	printf("%d ",start_point); // Imprime por onde comecamos.
	for(i=PathToSolution[0];i>=1;i--){ // Imprime todo o caminho que fizemos pelo labirinto ate o fim.
			printf("%d ",PathToSolution[i]);
	}
	printf("\n");
	return EXIT_SUCCESS;
}

void GenerateLabyrinth(char (*Lab)[7]){ // Esta funcao gera o labirinto.
	int L,C;
	for(L=0;L<7;L++){ // Para cada linha...
		for(C=0;C<7;C++){ // Para cada coluna...
			scanf(" %d",(int *)&Lab[L][C]); // Ler cada valor de entrada do labirinto. Obs.: O casting eh necessario porque usei uma matriz de char para o labirinto.
		}
	}
}

bool WalkTowards(int O_L,int O_C,int L,int C,int *Solution, char (*Lab)[7]){ // Esta funcao caminha pelo labirinto.
	bool ReturningValue=false;
	if(!Lab[L][C]){ // Se o caminho estiver livre (ou seja, for igual a zero), vamos caminhar.
		if(L+1!=O_L && L+1<=6 && !Lab[L+1][C]){ // Pode ir para cima?
			ReturningValue=WalkTowards(L,C,L+1,C,Solution,Lab); // Sim! Vamos subir entao.
			if(ReturningValue){ // O caminho para cima resulta no fim?
				Solution[++Solution[0]]=L*7+C; // Sim! Salva todo o caminho no nosso vetor de solucao.
				return ReturningValue; // Ja achamos o fim, pode encerrar todas as tentaivas aqui.
			}
		}
		if(L-1!=O_L && L-1>=0 && !Lab[L-1][C]){ // Pode ir para baixo?
			ReturningValue=WalkTowards(L,C,L-1,C,Solution,Lab);
			if(ReturningValue){
				Solution[++Solution[0]]=L*7+C;
				return ReturningValue;
			}
		}
		if(C+1!=O_C && C+1<=6 && !Lab[L][C+1]){ // Pode ir para a direita?
			ReturningValue=WalkTowards(L,C,L,C+1,Solution,Lab);
			if(ReturningValue){
				Solution[++Solution[0]]=L*7+C;
				return ReturningValue;
			}
		}
		if(C-1!=O_C && C-1>=0 && !Lab[L][C-1]){ // Pode ir para a esquerda?
			ReturningValue=WalkTowards(L,C,L,C-1,Solution,Lab);
			if(ReturningValue){
				Solution[++Solution[0]]=L*7+C;
				return ReturningValue;
			}
		}
		if((L==0) || (L==6) || (C==0) || (C==6)){ // Aparentemente nao pode ir para lugar nenhum. Chegamos no fim?
			Solution[++Solution[0]]=L*7+C; // Sim! Salva as coordenadas do fim.
			return true; // Chegamos no fim, entao retorna isso para que a funcao possa salvar todo o caminho que usamos para chegar aqui.
		}
	}
	return ReturningValue; // Nao podemos nos mover e nem chegamos no fim! Retorna que estamos em um beco sem fim para a funcao sair dele e pegar outro caminho!
}
