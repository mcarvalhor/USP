#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;


long recursion(long M, long k, vector< vector<long> > &items, long counter[][256]) {
	long i, N, aux;

	if(M < 0)
		return -1;
	else if(!k)
		return 0;

	counter[M][k] = -1;
	N = items[k - 1].size();

	for(i = 0; i < N; i++){
		if((aux=recursion(M - items[k - 1][i], k - 1, items, counter)) != -1)
			if(counter[M][k] < aux + items[k - 1][i])
				counter[M][k] = aux + items[k - 1][i];
	}

	return counter[M][k];
}


int main(int argc, char **argv) {
	vector< vector<long> > items;
	long i, j, k, N, M, C, K, S;
	long counter[256][256];

	cin >> N;
	for(i = 0; i < N; i++){
		cin >> M >> C;
		items = vector< vector<long> >(C);
		for(j = 0; j < C; j++) {
			cin >> K;
			items[j] = vector<long>(K);
			for(k = 0; k < K; k++){
				cin >> items[j][k];
			}
		}
		for(j = 0; j < 256; j++)
			for(k = 0; k < 256; k++)
				counter[j][k] = -2;
		S = recursion(M, C, items, counter);
		if(S < 0)
			cout << "no solution\n";
		else
			cout << S << '\n';
	}

	return EXIT_SUCCESS;
}


