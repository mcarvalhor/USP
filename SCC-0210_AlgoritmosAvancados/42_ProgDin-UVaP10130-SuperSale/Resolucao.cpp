#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;


long max_this_person_can_take(long limitW, long takingW, long takingP, vector< pair<long, long> > &objects, bool *visited) {
	long i, j, N, aux, counter;
	bool *visitedAux;

	if(takingW > limitW)
		return 0;
	if(takingW == limitW)
		return takingP;

	N = objects.size();
	visitedAux = (bool *) malloc(sizeof(bool) * N);

	for(i = counter = 0; i < N; i++){
		if(visited[i] == true)
			continue;
		for(j = 0; j < N; j++)
			visitedAux[j] = visited[j];
		visitedAux[i] = true;
		aux = max_this_person_can_take(limitW, takingW + objects[i].second, objects[i].first, objects, visitedAux);
		if(aux >= counter)
			counter = aux;
	}

	free(visitedAux);
	return takingP + counter;
}


int main(int argc, char **argv) {
	long i, j, T, N, P, W, G, S;
	vector< pair<long, long> > objects;
	vector<long> family;
	bool *visitedAux;

	cin >> T;
	for(i = 0; i < T; i++){

		cin >> N;
		objects = vector< pair<long, long> >(N);
		visitedAux = (bool *) malloc(sizeof(bool) * N);
		for(j = 0; j < N; j++) {
			cin >> P >> W;
			objects[j] = make_pair(P, W);
			visitedAux[j] = false;
		}

		cin >> G;
		family = vector<long>(G);
		for(j = 0; j < G; j++) {
			cin >> family[j];
		}

		for(S = j = 0; j < G; j++) {
			S += max_this_person_can_take(family[j], 0, 0, objects, visitedAux);
		}

		free(visitedAux);
		cout << S << '\n';

	}

	return EXIT_SUCCESS;
}


