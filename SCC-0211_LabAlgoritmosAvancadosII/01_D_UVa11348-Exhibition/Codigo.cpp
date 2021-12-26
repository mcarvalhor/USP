#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <unordered_map>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>



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

int main(int argc, char **argv) {
	long k, i, j, K, N, M, counter, aux;
	set<long> stamps[50];

	unordered_map<long, bool> unique;

	scanf("%ld", &K);
	for(k = 1; k <= K; k++) {

		unique.clear(); // Limpar map.

		// Escanear entradas, procurando por unique stamps e salvando no map.
		scanf("%ld", &N);
		for(i = 0; i < N; i++) { // Para cada amigo...
			scanf("%ld", &M);
			stamps[i].clear();
			for(j = 0; j < M; j++) { // Para cada stamp desse amigo...
				scanf("%ld", &aux);
				if(stamps[i].find(aux) != stamps[i].end()) {
					continue;
				}
				if(unique.find(aux) == unique.end()) {
					unique[aux] = true;
				} else {
					unique[aux] = false;
				}
				stamps[i].insert(aux);
			}
		}

		// Iterar chaves do map, removendo aquelas que não são "true".
		for(auto it = unique.cbegin(); it != unique.cend();) {
			if(it->second == false) {
				it = unique.erase(it);
			} else {
				it++;
			}
		}

		// Iterar e imprimir percentagem que cada amigo recebe.
		printf("Case %ld:", k);
		for(i = 0; i < N; i++) {
			counter = 0;
			for(auto it = stamps[i].begin(); it != stamps[i].end(); it++) {
				if(unique.find(*it) != unique.end()) {
					counter++;
				}
			}
			printf(" %.6lf%%", 100.0 * counter / (double) unique.size());
		}
		printf("\n");

	}
	
	return EXIT_SUCCESS;
}
