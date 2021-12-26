#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <algorithm>



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
	ll *prices, *dp;
	ll i, N;

	cin >> N;
	prices = (ll *) calloc(sizeof(ll), N + 10);
	dp = (ll *) calloc(sizeof(ll), N + 10);
	for(i = 0; i < N; i++) {
		cin >> prices[i];
	}

	for(i = N - 1; i >= 0; i--) {
		dp[i] = prices[i] + dp[i + 2];
		if(i + 1 < N) {
			dp[i] = max(dp[i], prices[i] + prices[i + 1] + dp[i + 4]);
		}
		if(i + 2 < N) {
			dp[i] = max(dp[i], prices[i] + prices[i + 1] + prices[i + 2] + dp[i + 6]);
		}
	}

	cout << dp[0] << endl;
	free(prices);
	free(dp);
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
