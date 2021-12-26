#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

#define N_DIAS_NO_MES 31
int main(){
	int i,dias[N_DIAS_NO_MES],maior=-1,iinserted,iindex=-1;
	for(i=1;i<=N_DIAS_NO_MES;i++){
		scanf("%d",&iinserted);
		if(iinserted>maior){
			maior=iinserted;
			dias[0]=i;
			iindex=0;
		}else if(iinserted==maior){
			dias[++iindex]=i;
		}
	}
	if(maior>-1){
		printf("%d\n",maior);
		for(i=0;i<=iindex;i++){
			printf("%d\n",dias[i]);
		}
	}
	return EXIT_SUCCESS;
}