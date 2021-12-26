#include <stdlib.h>

#include <algorithm>
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
	int N, i, val, greatest;
	
	cin >> N;
	greatest = 0;
	for(i = 0; i < N; i++) {

		cin >> val;

		if(val < greatest) {
			cout << "No" << endl;
			return EXIT_SUCCESS;
		} else if(val > greatest) {
			greatest = max(0, val - 1);
		}


	}

	cout << "Yes" << endl;

	return EXIT_SUCCESS;
}
