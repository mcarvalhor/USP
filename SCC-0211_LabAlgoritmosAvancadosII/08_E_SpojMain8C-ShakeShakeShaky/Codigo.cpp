#include <stdlib.h>
#include <string.h>
#include <limits.h>

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



bool solutionValid(ll solution, ll N, ll K, ll *candyBoxes) {
	ll i, sum;
	sum = 0;
	for(i = 0; i < N; i++) {
		sum += candyBoxes[i] / solution;
	}
	if(sum >= K) {
		return true;
	}
	return false;
}

void testCase() {
	ll N, K, i, start, end, middle, max, *candyBoxes;

	cin >> N >> K;
	candyBoxes = (ll *) malloc(sizeof(ll) * N);
	start = 1;
	end = 1e9;
	max = LLONG_MIN;
	for(i = 0; i < N; i++) {
		cin >> candyBoxes[i];
	}

	while(start <= end) {
		middle = (start + end) / 2;
		if(solutionValid(middle, N, K, candyBoxes)) {
			if(middle > max) {
				max = middle;
			}
			start = middle + 1;
		} else {
			end = middle - 1;
		}
	}

	if(max == LLONG_MIN) {
		cout << 0 << endl;
		return;
	}
	cout << max << endl;
}

int main(int argc, char **argv) {
	ll T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
