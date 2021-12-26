#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int N=0,i,j,*Int;
	char R,*Str;
	Str=(char *)malloc(0);
	do{
		R=getchar();
		if(R!='\n'){
			Str=(char *)realloc(Str,sizeof(char)*(++N));
			Str[N-1]=R;
		}
	}while(R!='x');
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			if(Str[i]>Str[j]){
				R=Str[i];
				Str[i]=Str[j];
				Str[j]=R;
			}
		}
		if(!((i+1)%4)){
			Int=(int *)&Str[i-3];
			printf("%d\n",*Int);
		}
	}
	free(Str);
	return EXIT_SUCCESS;
}
