#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char **argv){
	int i,N;
	float Space[2][2];
	float ScapeSpace[2];
	float Distancia[2];
	scanf("%d %f %f %f %f",&N,&Space[0][0],&Space[0][1],&Space[1][0],&Space[1][1]);
	for(i=0;i<N;i++){
		scanf("%f %f",&ScapeSpace[0],&ScapeSpace[1]);
		Distancia[0]=sqrt(pow(Space[0][1]-ScapeSpace[1],2)+pow(Space[0][0]-ScapeSpace[0],2));
		Distancia[1]=sqrt(pow(Space[1][1]-ScapeSpace[1],2)+pow(Space[1][0]-ScapeSpace[0],2));
		if(Distancia[0]<=(Distancia[1]/(float)2)){
			printf("O coelho pode escapar pelo buraco (%.3f,%.3f).\n",ScapeSpace[0],ScapeSpace[1]);
			return EXIT_SUCCESS;
		}
	}
	printf("O coelho nao pode escapar.\n");
	return EXIT_SUCCESS;
}