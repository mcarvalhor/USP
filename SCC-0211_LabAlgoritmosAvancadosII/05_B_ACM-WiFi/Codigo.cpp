#include <stdlib.h>
#include <string.h>

#include <iostream>



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

int int_cmp(const int *a, const int *b) {
	return *a - *b;
}

void testCase() {
	int left, right, middle, count, aux;
	int i, j, n, m, *M;
	char found;

	cin >> n >> m;

	M = (int *) malloc(sizeof(int) * m);
	for(i = 0; i < m; i++) {
		cin >> M[i];
		M[i] *= 2;
	}

	qsort(M, m, sizeof(int), (int (*)(const void *, const void *)) int_cmp); // Sort houses by street number.

	left = 0;
	right = (M[m - 1] - M[0]) / 2;
	while(left <= right) {
		middle = (left + right) / 2;
		count = 1;
		found = 1;
		aux = M[0] + middle;
		for(i = 0; i < m; i++) {
			if(abs(aux - M[i]) > middle) {
				count++;
				if(count > n) {
					found = 0;
					break;
				}
				aux = M[i] + middle;
			}
		}
		if(found == 1) {
			right = middle - 1;
		} else {
			left = middle + 1;
		}
	}

	printf("%.1lf", (right + 1) / (double) 2.0);
	cout << endl;
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
