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
	int i, j, k, aux, T, N;

	unordered_map<int, bool> alreadyFound;
	queue<int> list;

	scanf("%d", &T);
	for(; T > 0; T--) {

		alreadyFound = unordered_map<int, bool>(); // Resetar estruturas.
		list = queue<int>();

		scanf("%d", &N);

		for(i = 0; i < 2*N; i++) { // Ler entrada e verificar na hash e fila.
			scanf("%d", &aux);
			if(alreadyFound[aux] != true) {
				list.push(aux);
				alreadyFound[aux] = true;
			}
		}

		for(i = 0; i < N; i++) { // Imprimir resultado.
			aux = list.front();
			list.pop();
			printf("%d ", aux);
		}
		printf("\n");

	}
	
	return EXIT_SUCCESS;
}
