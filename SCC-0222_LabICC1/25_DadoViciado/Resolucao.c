#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,iinserted,entradas[6]={0,0,0,0,0,0},maior_frequencia=0;
	while(scanf("%d",&iinserted)!=EOF){
		if(iinserted>=1 && iinserted<=6){
			entradas[iinserted-1]++;
			if(entradas[iinserted-1]>maior_frequencia){
				maior_frequencia=entradas[iinserted-1];
			}
		}
	}
	for(i=0;i<6;i++){
		if(entradas[i]==maior_frequencia){
			printf("%d\n",i+1);
		}
	}
	printf("%d",maior_frequencia);
	return EXIT_SUCCESS;
}