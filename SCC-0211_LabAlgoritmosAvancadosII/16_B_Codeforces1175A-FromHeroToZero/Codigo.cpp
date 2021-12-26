#include <stdlib.h>

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



ll testCase(ll N, ll K) {
	ll count;
	if(K <= 1) {
		return N;
	}

	count = 0;
	while(N > 0) {
		if(K > N) {
			count += N;
			N = 0;
			continue;
		}
		if(N % K == 0) {
			N /= K;
			count++;
			continue;
		}
		count += N % K;
		N -= N % K;
	}

	return count;
}



int main(int argc, char **argv) {
	ll T, N, K;

	cin >> T;
	for(; T > 0; T--) {
		cin >> N >> K;
		cout << testCase(N, K) << endl;
	}

	return EXIT_SUCCESS;
}


