#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <bitset>
#include <cmath>

using namespace std;

bitset<10000> crivo;

void CalcularCrivo(){
	unsigned long i, j, N;
	N = crivo.size();
	crivo.set();
	crivo[1] = crivo[0] = 0;
	for(i = 2; i < N; i++){
		for(j = i + 1; j < N; j++){
			if(j % i == 0){
				crivo[j] = 0;
			}
		}
	}
}

bool IsPrime(unsigned long N){
	if(N < crivo.size())	return crivo[N];
	if(N % 2 == 0)	return false;
	unsigned long i, Max;
	Max = floor(sqrt(N));
	for(i = 3; i <= Max; i += 2){
		if(N % i == 0)	return false;
	}
	return true;
}

void ImprimirCrivo(){
	int i, N;
	N = crivo.size();
	for(i = 0; i < N; i++){
		cout << i << ": " << crivo[i] << "\n";
	}
}

void DecomporPrimos_Para(unsigned int N, unsigned int *vector){
	if(IsPrime(N)){
		vector[N]++;
		return;
	}
	int i, Max;
	Max = N;
	for(i = 2; i <= Max && N >= 2; i++){
		if(!IsPrime(i))	continue;
		while(N % i == 0){
			vector[i]++;
			N /= i;
		}
	}
}

int main(int argc, char **argv){
	unsigned int i, counter, N, *vector;
	CalcularCrivo();
	while(scanf("%d", &N) == 1){
		if(N == 0)	break;
		vector = (unsigned int *) calloc(1, sizeof(unsigned int) * (N+1));
		printf("%3d! =", N);
		for(i = 1; i <= N; i++)
			DecomporPrimos_Para(i, vector);
		for(i = counter = 0; i <= N; i++){
			if(!IsPrime(i))	continue;
			if((++counter)%16 == 0){
				printf("\n      ");
			}
			printf("%3d", vector[i]);
		}
		cout << '\n';
		free(vector);
	}
	return EXIT_SUCCESS;
}


