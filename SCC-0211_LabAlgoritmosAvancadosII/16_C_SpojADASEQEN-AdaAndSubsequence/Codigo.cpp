#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#define ALPHABET_START ('a')
#define ALPHABET_END ('z')
#define ALPHABET_SIZE (ALPHABET_END - ALPHABET_START + 1)



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

ll dp[2001][2001];
// Competitive Programming sometimes requires global variables - particularly not my favorite coding style, but they're not used to the best practices anyway...
// Better solution would be to use a MAP, but a MAP is very slow for this because they can't wait a few more miliseconds, so let's use this shi**y array solution.
// In Brazil country this is called "gambiarra", and competitive programmers know how to do this pretty well.



void testCase() {
	ll valueTable[ALPHABET_SIZE], i, j, N, M;
	char a[2001], b[2001];

	memset(dp, 0, sizeof(ll) * 2001 * 2001);
	cin >> N >> M;
	for(i = 0; i < ALPHABET_SIZE; i++) {
		cin >> valueTable[i];
	}

	//gets(a);
	//gets(b);
	cin >> a >> b;
	for(i = 0; i < N; i++) {
		for(j = 0; j < M; j++) {
			if(a[i] == b[j]) {
				dp[i + 1][j + 1] = dp[i][j] + valueTable[a[i] - ALPHABET_START];
				continue;
			}
			dp[i + 1][j + 1] = max(dp[i][j + 1], dp[i + 1][j]);
		}
	}
	cout << dp[N][M] << endl;
}

int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}


