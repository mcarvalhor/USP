#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>

using namespace std;


long solve(long camp, long remain, vector< vector<long> > &memo, vector<long> &dist) {
    if (camp == 0 && remain == 0)	return 0;
    if (remain == 0 || camp == 0)	return LONG_MAX;
    if (memo[camp][remain] != -1)	return memo[camp][remain];
	long prev, result;
    for (prev = remain - 1; prev < camp; prev++) {
        result = max(solve(prev, remain - 1, memo, dist), dist[camp] - dist[prev]);
        if (memo[camp][remain] == -1 || memo[camp][remain] > result){
            memo[camp][remain] = result;
		}
    }

    return memo[camp][remain];
}

int main(int argc, char **argv) {
    long i, N, K;

    cin >> N >> K;
	N += 2;
	K += 1;

	vector<long> dist(N);
	for (i = 1; i < N; i++) {
		cin >> dist[i];
		dist[i] += dist[i - 1];
	}

	vector< vector<long> > memo(N, vector<long>(K + 1, -1));

	cout << solve(N - 1, K, memo, dist) << "\n";

    return EXIT_SUCCESS;
}