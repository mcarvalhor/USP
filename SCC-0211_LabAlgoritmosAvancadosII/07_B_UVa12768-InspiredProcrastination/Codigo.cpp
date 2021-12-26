#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>



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


void find_and_print(int source, vector< vector< pair<int, int> > > G) {
	int i, N, u, v, w, *distances, *visited;
	bool *inQ;

	N = G.size();
	queue<int> Q = queue<int>();
	distances = (int *) malloc(sizeof(int) * N);
	visited = (int *) malloc(sizeof(int) * N);
	inQ = (bool *) malloc(sizeof(bool) * N);
	memset(visited, 0, sizeof(int) * N);
	for(i = 0; i < N; i++) {
		distances[i] = INT_MIN;
		inQ[i] = false;
	}
	distances[source] = 0;
	Q.push(source);

	while(Q.size() > 0) {
		u = Q.front();
		Q.pop();
		inQ[u] = false;
		visited[u]++;
		if(visited[u] > N) {
			cout << "Unlimited!" << endl;
			return;
		}
		for(i = 0; i < G[u].size(); i++) {
			v = G[u][i].first;
			w = G[u][i].second;
			if(distances[u] + w <= distances[v]) {
				continue;
			}
			distances[v] = distances[u] + w;
			if(inQ[v] == false) {
				inQ[v] = true;
				Q.push(v);
			}
		}
	}

	w = INT_MIN;
	for(i = 0; i < N; i++) {
		if(distances[i] > w) {
			w = distances[i];
		}
	}
	cout << w << endl;

	free(distances);
	free(visited);
	free(inQ);
}

void testCase(int N, int M) {
	int i, aux1, aux2, aux3;

	vector< vector< pair<int, int> > > G = vector< vector< pair<int, int> > >();
	for(i = 0; i < N; i++) {
		G.push_back( vector< pair<int, int> >() );
	}

	for(i = 0; i < M; i++) {
		cin >> aux1 >> aux2 >> aux3;
		aux1--;
		aux2--;
		G[aux1].push_back( make_pair(aux2, aux3) );
	}

	find_and_print(0, G);
}

int main(int argc, char **argv) {
	int N, M;

	cin >> N >> M;
	while(N > 0 && M > 0) {
		testCase(N, M);
		cin >> N >> M;
	}

	return EXIT_SUCCESS;
}
