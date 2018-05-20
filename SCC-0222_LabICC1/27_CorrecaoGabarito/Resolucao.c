#include <stdlib.h>
#include <stdio.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,j,N_questoes,N_alunos;
	scanf("%d %d",&N_questoes,&N_alunos);
	char Gabarito[N_questoes], Leitura;
	int Alunos[N_alunos];
	for(i=0;i<N_questoes;i++){
		scanf(" %c",&Gabarito[i]);
	}
	for(i=0;i<N_alunos;i++){
		Alunos[i]=0;
		for(j=0;j<N_questoes;j++){
			scanf(" %c",&Leitura);
			if(Leitura==Gabarito[j]){
				Alunos[i]++;
			}
		}
	}
	for(i=0;i<N_alunos;i++){
		printf("%.2f\n",(Alunos[i]/(float)sizeof(Gabarito))*10);
	}
	return EXIT_SUCCESS;
}
