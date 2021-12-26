#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>

using namespace std;

void DFS(long V, long Test, long *visited, vector< vector< pair<long, long> > > &G){
	long i, N;
	visited[V]++;
	N = G[V].size();
	for(i = 0; i < N; i++){
		if(visited[G[V][i].first] < 0 && G[V][i].first != Test){
			DFS(G[V][i].first, Test, visited, G);
		}
	}
}


int main(int argc, char **argv){
	long i, j, k, T, N, Aux, *visited, **results;
	cin >> T;
	for(i = 0; i < T; i++){
		cin >> N;
		vector< vector< pair<long, long> > > G(N);
		results = (long **) malloc(sizeof(long *) * N);
		visited = (long *) malloc(sizeof(long) * N);
		for(j = 0; j < N; j++){
			results[j] = (long *) malloc(sizeof(long) * N);
			for(k = 0; k < N; k++){
				cin >> Aux;
				if(Aux > 0){
					G[j].push_back(make_pair(k, 0));
				}
			}
		}
		for(j = 0; j < N; j++){
			memset(visited, -1, sizeof(long) * N);
			if(j)
				DFS(0, j, visited, G);
			for(k = 0; k < N; k++)
				results[j][k] = visited[k];
		}
		cout << "Case " << i + 1 << ":\n+";
		for(k = N * 2; k > 1; k--)
			cout << '-';
		cout << "+\n";
		for(j = 0; j < N; j++){
			cout << '|';
			for(k = 0; k < N; k++)
				cout << ((results[j][k] < 0) ? 'Y':'N') << '|';
			cout << "\n+";
			for(k = N * 2; k > 1; k--)
				cout << '-';
			cout << "+\n";
		}
		for(j = 0; j < N; j++)
			free(results[j]);
		free(visited);
		free(results);
	}
	return EXIT_SUCCESS;
}