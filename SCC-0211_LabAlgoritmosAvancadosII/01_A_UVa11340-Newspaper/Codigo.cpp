#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>



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



int main(int argc, char **argv) {
	double cust_table[256], total; /* assim eh mais rapido que hashtable/map */
	char line[10001 + 2], aux[10], *it;

	int i, j, k, N, K, M;

	scanf("%d", &N);
	for(i = 0; i < N; i++) {
		memset(cust_table, 0, sizeof(double) * 256); /* resetar valor dos char lá */
		total = 0.0; /* total */
		
		scanf("%d", &K);
		for(j = 0; j < K; j++) { /* ler tabelinha */
			scanf("%s", aux);
			scanf("%lf", &cust_table[*aux]);
		}
		
		scanf("%d", &M); fgets(line, 10001, stdin); /* fgets pra ignorar a linha do proprio numero */
		for(j = 0; j < M; j++) { /* fazer a soma aqui */
			fgets(line, 10001, stdin);
			for(it = line; *it != '\0'; it++) {
				total += cust_table[*it];
			}
		}
		
		/* imprime */
		printf("%0.2lf$\n", total/100.0);
		
	}
	
	return EXIT_SUCCESS;
}
