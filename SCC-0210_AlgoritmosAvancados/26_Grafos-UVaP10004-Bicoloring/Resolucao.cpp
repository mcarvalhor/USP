#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>

using namespace std;

bool DFS(long V, long Colour, long *visited, vector< vector< pair<long, long> > > &G){
	long i, N;
	visited[V] = Colour;
	N = G[V].size();
	for(i = 0; i < N; i++){
		if(visited[G[V][i].first] < 0 && !DFS(G[V][i].first, (Colour) ? 0:1, visited, G))
			return false;
		if(visited[G[V][i].first] == Colour)
			return false;
	}
	return true;
}


int main(int argc, char **argv){
	long i, N, K, A, B, *visited;
	while(cin >> N){
		if(N < 1)
			break;
		cin >> K;
		vector< vector< pair<long, long> > > G(N);
		visited = (long *) malloc(sizeof(long) * N);
		memset(visited, -1, sizeof(long) * N);
		for(i = 0; i < K; i++){
			cin >> A >> B;
			G[A].push_back(make_pair(B, 0));
			G[B].push_back(make_pair(A, 0));
		}
		if(DFS(0, 0, visited, G))
			cout << "BICOLORABLE.\n";
		else
			cout << "NOT BICOLORABLE.\n";
		free(visited);
	}
	return EXIT_SUCCESS;
}