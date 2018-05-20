#include <stdlib.h>
#include <stdio.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	char TabelaVelha[3][3]={{-1,-1,-1},{-1,-1,-1},{-1,-1,-1}}, vencedor=-1;
	int i=0,X,Y;
	while(scanf("%d %d",&X,&Y)>0){
		TabelaVelha[X][Y]=(i++)%2;
	}
	if(TabelaVelha[0][0]!=' ' && TabelaVelha[0][0]==TabelaVelha[1][1] && TabelaVelha[1][1]==TabelaVelha[2][2]){ // Venceu por diagonal?
		vencedor=TabelaVelha[0][0];
	}else if(TabelaVelha[0][2]!=' ' && TabelaVelha[0][2]==TabelaVelha[1][1] && TabelaVelha[1][1]==TabelaVelha[2][0] ){ // Venceu por diagonal?
		vencedor=TabelaVelha[0][2];
	}else if(TabelaVelha[0][0]!=' ' && TabelaVelha[0][0]==TabelaVelha[0][1] && TabelaVelha[0][1]==TabelaVelha[0][2]){ // Venceu na 1ª linha
		vencedor=TabelaVelha[0][0];
	}else if(TabelaVelha[1][0]!=' ' && TabelaVelha[1][0]==TabelaVelha[1][1] && TabelaVelha[1][1]==TabelaVelha[1][2]){ // Venceu na 2ª linha
		vencedor=TabelaVelha[1][0];
	}else if(TabelaVelha[2][0]!=' ' && TabelaVelha[2][0]==TabelaVelha[2][1] && TabelaVelha[2][1]==TabelaVelha[2][2]){ // Venceu na 3ª linha
		vencedor=TabelaVelha[2][0];
	}else if(TabelaVelha[0][0]!=' ' && TabelaVelha[0][0]==TabelaVelha[1][0] && TabelaVelha[1][0]==TabelaVelha[2][0]){ // Venceu na 1ª coluna
		vencedor=TabelaVelha[0][0];
	}else if(TabelaVelha[0][1]!=' ' && TabelaVelha[0][1]==TabelaVelha[1][1] && TabelaVelha[1][1]==TabelaVelha[2][1]){ // Venceu na 2ª coluna
		vencedor=TabelaVelha[0][1];
	}else if(TabelaVelha[0][2]!=' ' && TabelaVelha[0][2]==TabelaVelha[1][2] && TabelaVelha[1][2]==TabelaVelha[2][2]){ // Venceu na 3ª coluna
		vencedor=TabelaVelha[0][2];
	}
	if(vencedor==-1){
		printf("empate");
	}else{
		printf("%d",(int)vencedor);
	}
	return EXIT_SUCCESS;
}
