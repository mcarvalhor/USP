#include <stdlib.h>
#include <string.h>
#include <limits.h>
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

typedef struct {
	long time;
} BOOK;

int fcmp(const BOOK *a, const BOOK *b) {
	return a->time - b->time;
}


int main(int argc, char **argv) {
	long i, N, K, NA, NB, NBoth, aux1, aux2, aux3;
	BOOK *booksAlice, *booksBob, *booksBoth;

	scanf("%ld %ld", &N, &K); // Ler N e K.

	booksAlice = (BOOK *) malloc(sizeof(BOOK) * N); // Alocar
	booksBob = (BOOK *) malloc(sizeof(BOOK) * N);
	booksBoth = (BOOK *) malloc(sizeof(BOOK) * N);

	NA = NB = NBoth = 0;
	for(i = 0; i < N; i++) { // Ler livros de entrada.
		scanf("%ld %ld %ld", &aux1, &aux2, &aux3);
		if(aux2 && aux3) {
			booksBoth[NBoth++].time = aux1;
		} else if(aux2) {
			booksAlice[NA++].time = aux1;
		} else if(aux3) {
			booksBob[NB++].time = aux1;
		}
	}

	if(NA + NBoth < K || NB + NBoth < K) { // Verificar validade.
		printf("-1\n");
		return EXIT_SUCCESS;
	}

	qsort(booksAlice, NA, sizeof(BOOK), (int (*)(const void *, const void *)) fcmp); // Ordenar por tempo de leitura crescente.
	qsort(booksBob, NB, sizeof(BOOK), (int (*)(const void *, const void *)) fcmp);
	qsort(booksBoth, NBoth, sizeof(BOOK), (int (*)(const void *, const void *)) fcmp);

	for(i = 1; i < NA; i++) {
		booksAlice[i].time += booksAlice[i - 1].time;
	}

	for(i = 1; i < NB; i++) {
		booksBob[i].time += booksBob[i - 1].time;
	}

	for(i = 1; i < NBoth; i++) {
		booksBoth[i].time += booksBoth[i - 1].time;
	}

	i = 0;
	aux3 = INT_MAX;
	while(i <= NBoth && i <= K) {
		aux1 = (i > 0) ? booksBoth[i - 1].time : 0;
		aux2 = K - i;
		if(NA >= aux2 && NB >= aux2){
			aux1 += (aux2 > 0) ? (booksAlice[aux2 - 1].time + booksBob[aux2 - 1].time) : 0;
			aux3 = (aux3 < aux1) ? aux3 : aux1;
		}
		i++;
	}

	printf("%ld\n", aux3);

	free(booksAlice);
	free(booksBob);
	free(booksBoth);
	return EXIT_SUCCESS;
}
