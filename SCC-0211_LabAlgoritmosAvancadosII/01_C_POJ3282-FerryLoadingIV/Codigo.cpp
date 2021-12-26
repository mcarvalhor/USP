#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <vector>
#include <stack>
#include <queue>



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

enum { left = 0, right = 1 };

int main(int argc, char **argv) {
	long i, j, N, M, counter;
	double L, l, carL;
	int ferryAt;
	char aux[20];

	queue<double> carQueues[2];

	scanf("%ld", &N);
	for(i = 0; i < N; i++) {

		scanf("%lf %ld", &L, &M);
		ferryAt = left;
		counter = 0;
		L *= 100.0;
		l = L;

		carQueues[left] = queue<double>();
		carQueues[right] = queue<double>();

		for(j = 0; j < M; j++) { // Para cada carro nas filas...
			scanf("%lf %s", &carL, aux);
			if(*aux == 'l' || *aux == 'L') { // Carro chegou na fila da esquerda...
				carQueues[left].push(carL);
			} else { // Carro chegou na fila da direita...
				carQueues[right].push(carL);
			}
		}

		while(!carQueues[left].empty() || !carQueues[right].empty()) { // Processar filas...
			while(!carQueues[ferryAt].empty() && l - carQueues[ferryAt].front() >= 0) { // Entrar no ferry enquanto couber...
				l -= carQueues[ferryAt].front();
				carQueues[ferryAt].pop();
			}
			// Não cabe mais OU acabou a fila.
			ferryAt = (ferryAt + 1) % 2; // ferryAt = !ferryAt;
			l = L; // Descarregar.
			counter++;
		}

		printf("%ld\n", counter);

	}
	
	return EXIT_SUCCESS;
}
