#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>
#include <bitset>
#include <cmath>
#include <map>

using namespace std;

bitset<5000> crivo;
map<long, long> extra_crivo;

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
	if(extra_crivo[N] == 2)	return true;
	if(N % 2 == 0)	return false;
	unsigned long i, Max;
	Max = floor(sqrt(N));
	for(i = 3; i <= Max; i += 2){
		if(N % i == 0)	return false;
	}
	extra_crivo[N] = 2;
	return true;
}

void ImprimirCrivo(){
	int i, N;
	N = crivo.size();
	for(i = 0; i < N; i++){
		cout << i << ": " << crivo[i] << "\n";
	}
}

void DecomporPrimos_Para(unsigned int N, map <long, long>&vector){
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

long lcm(long A, long B){
	long Min;
	Min = (A > B) ? B : A;
	for(; Min > 0; Min--){
		if(A % Min == 0 && B % Min == 0)	break;
	}
	return (A*B) / Min;
}

long LCM_cardinality(long N){
	long i, j, counter = 0;
	if(N % 2 == 0){
		for(i = 1; i <= N; i++){
			if(lcm(i, N) == N){
				for(j = i; j <= N; j++){
					if(lcm(j, N) == N){
						counter++;
					}
				}
			}
		}
	}else{
		for(i = 1; i <= N; i += 2){
			if(lcm(i, N) == N){
				for(j = i; j <= N; j += 2){
					if(lcm(j, N) == N){
						counter++;
					}
				}
			}
		}
	}
	return counter;
}


int main(int argc, char **argv){
	long Aux, N;
	CalcularCrivo();
	cin >> Aux;
	while(Aux != 0){
		map <long, long>vector;
		map <long, long>::iterator it;
		cout << Aux << " ";
		N = 0;
		DecomporPrimos_Para(Aux, vector);
		for(it = vector.begin(); it != vector.end(); it++){
			N += LCM_cardinality(it->first) * it->second;
		}
		cout << N-1 << '\n';
		cin >> Aux;
	}
}
