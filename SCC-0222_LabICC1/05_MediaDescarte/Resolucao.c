#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*
Matheus
mcarvalhor@usp.br
*/

int main(){
	double a[4],menor=0,media=0;
	int i;
	scanf("%lf %lf %lf %lf",&a[0],&a[1],&a[2],&a[3]);
	menor=a[0];
	for(i=0;i<4;i++){
		media+=a[i];
		if(a[i]<menor){
			menor=a[i];
		}
	}
	media=(media-menor)/3.0;
	printf("%.4lf",media);
	return EXIT_SUCCESS;
}
