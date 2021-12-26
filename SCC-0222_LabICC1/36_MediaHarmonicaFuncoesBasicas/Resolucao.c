#include <stdio.h>
#include <stdlib.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

int readInt(){
	int R;
	scanf("%d",&R);
	return R;
}

double readDouble(){
	double R;
	scanf("%lf",&R);
	return R;
}

void printDouble(double val){
	printf("%.2lf\n",val);
}

int main(){
	int i,N=readInt();
	double MHA=0;
	for(i=0;i<N;i++){
		MHA+=1/(double)(readDouble()+1);
	}
	MHA=N/(double)MHA;
	printDouble(--MHA);
	return EXIT_SUCCESS;
}
