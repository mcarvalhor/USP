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



ll gcd(ll a, ll b) {
	if(b > 0) {
		return gcd(b, a % b);
	}
	return a;
}

void testCase() {
	ll S, A, N, takes;
	bool Sturn;

	cin >> S >> A >> N;
	Sturn = true;

	while(N > 0) {
		if(Sturn) {
			takes = gcd(S, N);
			N -= takes;
		} else {
			takes = gcd(A, N);
			N -= takes;
		}
		Sturn = !Sturn;
	}

	if(!Sturn) {
		cout << '0';
	} else {
		cout << '1';
	}
	cout << endl;
}



int main(int argc, char **argv) {
	ll T, N, K;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}


