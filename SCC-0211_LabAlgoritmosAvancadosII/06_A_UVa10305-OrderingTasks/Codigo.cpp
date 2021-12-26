#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>
#include <stack>



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



void dfs(int n, stack<int> &sortedVertex, vector<char> &markStatus, vector< vector<int> > &G) {
	int i;
	if(markStatus[n] == 2) {
		return;
	}
	if(markStatus[n] == 1) {
		return; // STOP NOT A DAG
	}
	markStatus[n] = 1;
	for(i = 0; i < G[n].size(); i++) {
		dfs(G[n][i], sortedVertex, markStatus, G);
	}
	markStatus[n] = 2;
	sortedVertex.push(n);
}

void testCase(int N, int M) {
	int i, m, n;

	vector< vector<int> > G = vector< vector<int> >();
	vector<char> markStatus = vector<char>();
	stack<int> sortedVertex = stack<int>();

	for(i = 0; i < N; i++) {
		G.push_back( vector<int>() );
		markStatus.push_back( 0 );
	}

	for(i = 0; i < M; i++) {
		cin >> m >> n;
		G[m - 1].push_back(n - 1);
	}

	for(i = 0; i < N; i++) {
		if(markStatus[i] == 2) {
			continue;
		}
		dfs(i, sortedVertex, markStatus, G);
	}

	while(sortedVertex.size() > 0) {
		cout << sortedVertex.top() + 1 << ' ';
		sortedVertex.pop();
	}
	cout << endl;
}

int main(int argc, char **argv) {
	int N, M;

	cin >> N >> M;
	while(N > 0) {
		testCase(N, M);
		cin >> N >> M;
	}

	return EXIT_SUCCESS;
}
