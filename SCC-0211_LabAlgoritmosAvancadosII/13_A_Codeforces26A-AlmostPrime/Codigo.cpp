#include <stdlib.h>

#include <iostream>
#include <vector>

#define PRIME_TABLE_MAXSIZE 3001



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



vector<ll> gen_primes_table() {
	bool isPrime;
	ll i, j;

	vector<ll> primeList = vector<ll>();
	primeList.push_back(2);

	for(i = 3; i <= PRIME_TABLE_MAXSIZE; i += 2) {
		isPrime = true;
		for(auto prime = primeList.begin(); prime != primeList.end(); prime++) {
			if(i % *prime != 0) {
				continue;
			}
			isPrime = false;
			break;
		}
		if(isPrime) {
			primeList.push_back(i);
		}
	}

	return primeList;
}

void testCase() {
	ll i, N, countOfPrimes, countOfAlmostPrimes;

	cin >> N;
	countOfAlmostPrimes = 0;
	vector<ll> primeList = gen_primes_table();

	for(i = 1; i <= N; i++) {
		countOfPrimes = 0;
		for(auto prime = primeList.begin(); prime != primeList.end(); prime++) {
			if(*prime > i) {
				break;
			}
			if(i % *prime != 0) {
				continue;
			}
			countOfPrimes++;
		}
		if(countOfPrimes == 2) {
			countOfAlmostPrimes++;
		}
	}

	cout << countOfAlmostPrimes << endl;
}



int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}


