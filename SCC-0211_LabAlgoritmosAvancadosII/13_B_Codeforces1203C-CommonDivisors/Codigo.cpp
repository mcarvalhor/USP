#include <stdlib.h>

#include <iostream>
#include <set>



/*
 *      ==    AlgAvançados   ==
 *
 *      Matheus Carvalho Raimundo (Nº 10369014) - based on https://www.geeksforgeeks.org/common-divisors-of-n-numbers/
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
	if (a == 0) {
		return b;
	}
	return gcd(b % a, a);
}

ll count_divisors(ll *A, ll N) {
	ll i, j, gcd_all;

	set<ll> list = set<ll>();

	gcd_all = A[0];
	for(i = 1; i < N; i++) {
		gcd_all = gcd(gcd_all, A[i]);
	}

	for(i = 1; i*i <= gcd_all; i++) {
		if(gcd_all % i != 0) {
			continue;
		}
		list.insert(i);
		if(gcd_all / i == i) {
			continue;
		}
		list.insert(gcd_all / i);
	}

	return list.size();
}

void testCase() {
	ll i, N, *A;

	cin >> N;
	A = (ll *) malloc(sizeof(ll) * N);
	for(i = 0; i < N; i++) {
		cin >> A[i];
	}

	cout << count_divisors(A, N) << endl;
	free(A);
}



int main(int argc, char **argv) {

	testCase();

	return EXIT_SUCCESS;
}


