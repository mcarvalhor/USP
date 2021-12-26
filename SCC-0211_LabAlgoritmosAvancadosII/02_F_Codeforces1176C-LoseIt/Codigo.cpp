#include <stdlib.h>
#include <string.h>

#include <unordered_map>
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

int main(int argc, char **argv) {
	int N, i, k, val;
	int counter[6];

	unordered_map<int, int> possibleValues = {
		{4,  0},
		{8,  1},
		{15, 2},
		{16, 3},
		{23, 4},
		{42, 5}
	};

	cin >> N;
	memset(counter, 0, sizeof(int) * 6);
	for(k = 0; k < N; k++) {

		cin >> val;
		i = possibleValues[val];

		if(i == 0) {
			counter[i]++;
		} else if(counter[i - 1] > 0) {
			counter[i - 1]--;
			counter[i]++;
		}

	}

	val = counter[5] * 6;
	cout << (N - val) << endl;

	return EXIT_SUCCESS;
}
