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

int find_NA(int start, int end, int N, int A, int B, int K) {
	int NB, current_K, middle, aux;
	if(start > end) {
		return -1;
	}

	middle = (start + end) / 2;
	current_K = K - middle * A;
	NB = current_K / B;
	NB = max(NB - 1, 0);
	current_K -= NB * B;
	if(current_K > B) {
		current_K -= B;
		NB++;
	}

	if(K <= 0 || middle + NB < N) {
		return find_NA(start, middle - 1, N, A, B, K);
	}
	aux = find_NA(middle + 1, end, N, A, B, K);
	if(aux >= 0) {
		return aux;
	}
	return middle;
}

void testCase() {
	int K, N, A, B;
	int NA_max;

	cin >> K >> N >> A >> B;

	NA_max = K / A;
	NA_max = max(NA_max - 1, 0);
	if(K - NA_max * A > A) {
		NA_max++;
	}

	cout << min(find_NA(0, NA_max, N, A, B, K), N) << endl;
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
