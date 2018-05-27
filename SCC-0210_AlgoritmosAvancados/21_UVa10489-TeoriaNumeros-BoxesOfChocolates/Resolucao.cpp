#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;


int main(int argc, char **argv){
	int i, j, k, T, N, B, K, Sum, InternalSum, Aux;
	cin >> T;
	for(i = 0; i < T; i++){
		cin >> N >> B;
		for(j = Sum = 0; j < B; j++) {
			cin >> K;
			for(k = 0, InternalSum = 1; k < K; k++) {
				cin >> Aux;
				InternalSum *= Aux;
				InternalSum %= N;
			}
			Sum += InternalSum;
			Sum %= N;
		}
		cout << Sum << '\n';
	}
	return EXIT_SUCCESS;
}



