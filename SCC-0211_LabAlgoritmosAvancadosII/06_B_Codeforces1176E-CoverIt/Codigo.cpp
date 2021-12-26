#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <tuple>
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



void dfs(int current, int s, int *it1, int *it2, char *status, int N, vector< vector<int> > &G) {
	int i;
	status[current] = s;

	if(s == 1) {
		(*it1)++;
	} else {
		(*it2)++;
	}

	for(i = 0; i < G[current].size(); i++) {
		if(status[G[current][i]] < 1) {
			dfs(G[current][i], 3 - s, it1, it2, status, N, G);
		}
	}
}

void testCase() {
	int M, N, K, a, b, i, it, it1, it2;
	char *status;

	cin >> N >> M;
	vector< vector<int> > G = vector< vector<int> >();
	status = (char *) malloc(sizeof(char) * N);
	memset(status, 0, sizeof(char) * N);
	for(i = 0; i < N; i++) {
		G.push_back( vector<int>() );
	}

	for(i = 0; i < M; i++) {
		cin >> a >> b;
		G[a - 1].push_back(b - 1);
		G[b - 1].push_back(a - 1);
	}

	it1 = it2 = 0;
	dfs(1, 1, &it1, &it2, status, N, G);

	cout << min(it1, it2) << endl;
	for(i = 0; i < N; i++) {
		if((it1 <= it2 && status[i] == 1) || (it1 > it2 && status[i] == 2)) {
			cout << i + 1 << ' ';
		}
	}
	cout << endl;

	free(status);
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
