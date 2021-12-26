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


void dijkstra(long long source, long long *distances, vector< vector< pair<long long, long long> > > G) {
	long long i, j, N, u, v, w, minV, minI;

	N = G.size();
	priority_queue< pair<long long, long long> > Q = priority_queue< pair<long long, long long> >();
	for(i = 0; i < N; i++) {
		distances[i] = LLONG_MAX;
	}
	distances[source] = 0;
	Q.push( make_pair(0, source) );

	while(Q.size() > 0) {
		u = Q.top().second;
		Q.pop();
		for(j = 0; j < G[u].size(); j++) {
			v = G[u][j].first;
			w = G[u][j].second;
			if(distances[u] + w < distances[v]) {
				distances[v] = distances[u] + w;
				Q.push( make_pair(-distances[v], v) );
			}
		}
	}

}

void testCase() {
	long long N, M, T, K, P, *distances;
	long long i, aux1, aux2, aux3;
	bool *is_pine;

	cin >> N >> M >> T >> K >> P;
	vector< vector< pair<long long, long long> > > G = vector< vector< pair<long long, long long> > >();
	is_pine = (bool *) malloc(sizeof(bool) * N);
	distances = (long long *) malloc(sizeof(long long) * N);
	memset(distances, 0, sizeof(long long) * N);
	for(i = 0; i < N; i++) {
		is_pine[i] = false;
		distances[i] = LLONG_MAX;
		G.push_back( vector< pair<long long, long long> >() );
	}

	for(i = 0; i < P; i++) {
		cin >> aux1;
		aux1--;
		is_pine[aux1] = true;
	}

	T *= 60;
	for(i = 0; i < M; i++) {
		cin >> aux1 >> aux2 >> aux3;
		aux1--;
		aux2--;
		aux3 *= 60; // Minutes to Seconds
		if(is_pine[aux2]) {
			G[aux1].push_back( make_pair(aux2, aux3 + K) );
		} else {
			G[aux1].push_back( make_pair(aux2, aux3) );
		}
		/*if(is_pine[aux1]) {
			G[aux2].push_back( make_pair(aux1, aux3 + K) );
		} else {
			G[aux2].push_back( make_pair(aux1, aux3) );
		}*/
	}

	free(is_pine);
	dijkstra(0, distances, G);

	if(distances[N - 1] <= T && distances[N - 1] != LLONG_MAX) {
		cout << distances[N - 1] << endl; // Yeah they don't mention that, but they want this result in seconds...
	} else {
		cout << -1 << endl;
	}

	free(distances);
}

int main(int argc, char **argv) {
	int T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
