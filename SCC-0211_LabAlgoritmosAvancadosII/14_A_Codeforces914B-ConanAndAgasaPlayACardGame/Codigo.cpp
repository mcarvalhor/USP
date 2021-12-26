#include <stdlib.h>

#include <iostream>
#include <map>



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
	ll i, N, aux;
	bool won;

	cin >> N;
	map<ll, ll> A = map<ll, ll>();
	for(i = 0; i < N; i++) {
		cin >> aux;
		A[aux]++;
	}

	won = false;
	for(auto& it: A) {
		if(it.second % 2 == 0) {
			continue;
		}
		won = true;
		break;
	}

	if(won) {
		cout << "Conan";
	} else {
		cout << "Agasa";
	}
	cout << endl;
}



int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}


