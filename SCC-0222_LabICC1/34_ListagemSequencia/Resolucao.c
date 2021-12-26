#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int positionof(float number, float *numbers, int numbers_count){
	int i;
	for(i=0;i<numbers_count;i++){
		if(numbers[i]==number){
			return i;
		}
	}
	return -1;
}

int main(){
	int i,j,R,N,n=0;
	float S;
	scanf("%d",&N);
	float Number_Array[N];
	int Count_Array[N];
	for(i=0;i<N;i++){
		Count_Array[i]=0;
		scanf("%f",&Number_Array[n]);
		R=positionof(Number_Array[n],Number_Array,N);
		if(R==n){
			Count_Array[n++]++;
		}else{
			Count_Array[R]++;
		}
	}
	for(i=0;i<n;i++){
		for(j=i+1;j<n;j++){
			if(Number_Array[i]>Number_Array[j]){
				S=Number_Array[i];
				Number_Array[i]=Number_Array[j];
				Number_Array[j]=S;
				R=Count_Array[i];
				Count_Array[i]=Count_Array[j];
				Count_Array[j]=R;
			}
		}
		printf("%.1f %d\n",Number_Array[i],Count_Array[i]);
	}
	return EXIT_SUCCESS;
}
