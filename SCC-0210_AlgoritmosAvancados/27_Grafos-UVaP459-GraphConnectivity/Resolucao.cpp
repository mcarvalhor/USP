#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <utility>
#include <vector>

using namespace std;

void DFS(long V, long *visited, vector< vector< pair<long, long> > > &G){
	long i, N;
	visited[V] = 2;
	N = G[V].size();
	for(i = 0; i < N; i++){
		if(visited[G[V][i].first] < 0){
			DFS(G[V][i].first, visited, G);
		}
	}
}


int main(int argc, char **argv){
	long i, j, N, K, counter, *visited;
	char aux[200];
	cin >> N;
	for(i = 0; i < N; i++){
		cin >> aux;
		K = aux[0] - 'A' + 1;
		vector< vector< pair<long, long> > > G(K);
		visited = (long *) malloc(sizeof(long) * K);
		memset(visited, -1, sizeof(long) * K);
		fgets(aux, sizeof(aux)/sizeof(char), stdin);
		fgets(aux, sizeof(aux)/sizeof(char), stdin);
		while(aux[0] >= 'A' && aux[0] <= 'Z'){
			G[aux[0] - 'A'].push_back(make_pair(aux[1] - 'A', 0));
			G[aux[1] - 'A'].push_back(make_pair(aux[0] - 'A', 0));
			aux[0] = '\0';
			fgets(aux, sizeof(aux)/sizeof(char), stdin);
		}
		for(j = counter = 0; j < K; j++){
			if(visited[j] >= 0)
				continue;
			DFS(j, visited, G);
			counter++;
		}
		if(i)
			cout << '\n';
		cout << counter << '\n';
	}
	return EXIT_SUCCESS;
}