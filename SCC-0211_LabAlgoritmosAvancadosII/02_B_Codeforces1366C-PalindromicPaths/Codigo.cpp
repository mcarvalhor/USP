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
	long i, j, n, m, tmp, c, *cMatrix;
	char *matrix;
	int aux, T;

	scanf("%d", &T);
	for(; T > 0; T--) {

		scanf("%ld %ld", &n, &m); // Ler

		matrix = (char *) malloc(sizeof(char) * n * m); // Alocar
		cMatrix = (long *) malloc(sizeof(long) * n * m);
		memset(cMatrix, 0, sizeof(long) * (n + m)); // Set para zero

		for(i = 0; i < n; i++) { // Ler matriz
			for(j = 0; j < m; j++) {
				scanf("%d", &aux);
				matrix[i * m + j] = aux;
			}
		}

		for(i = 0; i < n; i++) { // Calcular
			for(j = 0; j < m; j++) {
				tmp = min(i + j, n + m - i - j - 2);
				if(tmp < (n + m - 1)/2) {
					cMatrix[tmp * 2 + matrix[i * m + j]]++;
				}
			}
		}

		for(i = c = 0; i < (n + m - 1) / 2; i++) { // Somar
			c += min(cMatrix[i * 2 + 0], cMatrix[i * 2 + 1]);
		}

		printf("%ld\n", c); // Imprimir

		free(matrix); // Free
		free(cMatrix);

	}
	
	return EXIT_SUCCESS;
}
