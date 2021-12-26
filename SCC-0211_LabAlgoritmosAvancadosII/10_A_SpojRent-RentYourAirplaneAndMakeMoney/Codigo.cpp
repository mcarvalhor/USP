#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <algorithm>
#include <vector>



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
typedef struct __plane_travel {
	int startTime, finishTime, cost;

	bool operator()(const struct __plane_travel &a, int b){
		return a.finishTime > b;
	}

	bool operator()(int b, const struct __plane_travel &a){
		return a.finishTime > b;
	}

	bool operator()(const struct __plane_travel &a, const struct __plane_travel &b){
		return a.finishTime < b.finishTime;
	}

} plane_travel;



int cmp(const plane_travel *A, const plane_travel *B) {
	return A->finishTime - B->finishTime;
}

void testCase() {
	int i, j, N;
	bool found;

	cin >> N;
	vector<plane_travel> travels = vector<plane_travel>(N);
	vector<int> dp = vector<int>(N);
	for(i = 0; i < N; i++) {
		cin >> travels[i].startTime >> travels[i].finishTime >> travels[i].cost;
		travels[i].finishTime += travels[i].startTime;
	}

	sort(travels.begin(), travels.end(), plane_travel());
	dp[0] = travels[0].cost;
	for(i = 1; i < N; i++) {
		found = false;
		j = upper_bound(travels.begin(), travels.begin() + i, travels[i].startTime, plane_travel()) - travels.begin();
		j--;
		if(i - 1 == j && travels[i - 1].finishTime > travels[i].startTime) {
			dp[i] = max(dp[i - 1], travels[i].cost);
		} else {
			dp[i] = max(dp[i - 1], dp[j] + travels[i].cost);
		}
	}

	cout << dp[N - 1] << endl;
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
