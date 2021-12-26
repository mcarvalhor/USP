#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int main(){
	int i,j,k,R;
	int Dim[2][2]={{0,0},{0,0}};
	scanf("%d %d",&Dim[0][0],&Dim[0][1]);
	int **MatA=(int **)malloc(sizeof(int *)*Dim[0][0]);
	for(i=0;i<Dim[0][0];i++){
		MatA[i]=(int *)malloc(sizeof(int)*Dim[0][1]);
		for(j=0;j<Dim[0][1];j++){
			scanf("%d",&MatA[i][j]);
		}
	}
	scanf("%d %d",&Dim[1][0],&Dim[1][1]);
	int **MatB=(int **)malloc(sizeof(int *)*Dim[1][0]);
	for(i=0;i<Dim[1][0];i++){
		MatB[i]=(int *)malloc(sizeof(int)*Dim[1][1]);
		for(j=0;j<Dim[1][1];j++){
			scanf("%d",&MatB[i][j]);
		}
	}
	for(i=0;i<Dim[0][0];i++){
		for(j=0;j<Dim[1][1];j++){
			for(R=0,k=0;k<Dim[1][0];k++){
				R+=MatA[i][k]*MatB[k][j];
			}
			if(j){
				printf(" %d",R);
			}else{
				printf("%d",R);
			}
		}
		printf("\n");
	}
	for(i=0;i<Dim[0][0];i++){
		free(MatA[i]);
	}
	for(i=0;i<Dim[1][0];i++){
		free(MatB[i]);
	}
	free(MatA);
	free(MatB);
	return EXIT_SUCCESS;
}
