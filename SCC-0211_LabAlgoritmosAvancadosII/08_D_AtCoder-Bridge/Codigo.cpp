#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>



/*
 *      ==    AlgAvançados   ==
 *
 *      Matheus Carvalho Raimundo (Nº 10369014) - based on https://www.geeksforgeeks.org/bridge-in-a-graph/
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/



using namespace std;
typedef long long ll;



void dfs(ll u, bool *visited, ll *disc, ll *low, ll *parent, ll *time, ll *sum, vector< vector<ll> > &G) {
	ll v;

	visited[u] = true;
	disc[u] = low[u] = ++(*time);
	for(auto it = G[u].begin(); it != G[u].end(); it++) {
		v = *it;
		if(visited[v]) {
			if(v != parent[u]) {
				low[u] = min(low[u], disc[v]);
			}
			continue;
		}
		parent[v] = u;
		dfs(v, visited, disc, low, parent, time, sum, G);
		low[u] = min(low[u], low[v]);
		if(low[v] > disc[u]) {
			(*sum)++;
		}
	}
}

void testCase() {
	ll N, M, i, aux1, aux2, time, sum;
	ll *disc, *low, *parent;
	bool *visited;

	cin >> N >> M;
	vector< vector<ll> > G = vector< vector<ll> >();
	visited = (bool *) malloc(sizeof(bool) * N);
	parent = (ll *) malloc(sizeof(ll) * N);
	disc = (ll *) malloc(sizeof(ll) * N);
	low = (ll *) malloc(sizeof(ll) * N);
	for(i = 0; i < N; i++) {
		G.push_back( vector<ll>() );
		visited[i] = false;
		parent[i] = -1;
		disc[i] = 0;
		low[i] = 0;
	}

	for(i = 0; i < M; i++) {
		cin >> aux1 >> aux2;
		aux1--;
		aux2--;
		G[aux1].push_back(aux2);
		G[aux2].push_back(aux1);
	}

	time = sum = 0;
	for(i = 0; i < N; i++) {
		if(!visited[i]) {
			dfs(i, visited, disc, low, parent, &time, &sum, G);
		}
	}

	cout << sum << endl;
}

int main(int argc, char **argv) {
	ll T;

	T = 1;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
