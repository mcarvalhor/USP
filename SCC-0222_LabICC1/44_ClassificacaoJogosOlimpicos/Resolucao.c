#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

typedef struct Country_Score{
	char Pais[4];
	int N_Ouro;
	int N_Prata;
	int N_Bronze;
} SCORE;
int main(){
	int i,j,N;
	SCORE R;
	scanf("%d",&N);
	SCORE JogosOlimpicos[N];
	for(i=0;i<N;i++){
		scanf("%s %d %d %d",JogosOlimpicos[i].Pais,&JogosOlimpicos[i].N_Ouro,&JogosOlimpicos[i].N_Prata,&JogosOlimpicos[i].N_Bronze);
		getchar();
	}
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			if( (JogosOlimpicos[j].N_Ouro>JogosOlimpicos[i].N_Ouro) || (JogosOlimpicos[j].N_Ouro==JogosOlimpicos[i].N_Ouro && JogosOlimpicos[j].N_Prata>JogosOlimpicos[i].N_Prata) || (JogosOlimpicos[j].N_Ouro==JogosOlimpicos[i].N_Ouro && JogosOlimpicos[j].N_Prata==JogosOlimpicos[i].N_Prata && JogosOlimpicos[j].N_Bronze>JogosOlimpicos[i].N_Bronze)){
				R=JogosOlimpicos[j];
				JogosOlimpicos[j]=JogosOlimpicos[i];
				JogosOlimpicos[i]=R;
			}
		}
		printf("%s %d %d %d\n",JogosOlimpicos[i].Pais,JogosOlimpicos[i].N_Ouro,JogosOlimpicos[i].N_Prata,JogosOlimpicos[i].N_Bronze);
	}
	return EXIT_SUCCESS;
}
