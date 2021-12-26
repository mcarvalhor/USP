#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>



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
	ll N, i, aux, sum;

	cin >> N;
	unordered_map<ll, ll> count = unordered_map<ll, ll>();
	for(i = 0; i < N; i++) {
		cin >> aux;
		count[aux]++;
	}

	sum = 0;
	for(auto it = count.begin(); it != count.end(); it++) {
		if(it->first > it->second) {
			sum += it->second;
			continue;
		}
		sum += it->second - it->first;
	}

	cout << sum << endl;
}

int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
