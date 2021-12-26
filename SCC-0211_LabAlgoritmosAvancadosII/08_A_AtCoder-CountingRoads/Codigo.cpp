#include <stdlib.h>
#include <string.h>

#include <iostream>



/*
 *      ==    AlgAvançados   ==
 *
 *      Matheus Carvalho Raimundo (Nº 10369014)
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/



using namespace std;
typedef long long ll;



void testCase() {
	ll N, M, i, aux1, aux2, *count;

	cin >> N >> M;
	count = (ll *) malloc(sizeof(ll) * N);
	memset(count, 0, sizeof(ll) * N);
	for(i = 0; i < M; i++) {
		cin >> aux1 >> aux2;
		aux1--;
		aux2--;
		count[aux1]++;
		count[aux2]++;
	}

	for(i = 0; i < N; i++) {
		cout << count[i] << endl;
	}
}

int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
