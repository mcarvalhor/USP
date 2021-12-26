#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <string>



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



int count_moves(string S, char c) {
	int i, middle, count_1, count_2;
	if(S.size() < 1) {
		return 0;
	}
	if(S.size() == 1) {
		return S[0] != c;
	}

	middle = S.size() /2;
	count_1 = count_2 = 0;

	// First try the first-half as "== c".
	for(i = 0; i < middle; i++) {
		if(S[i] != c) {
			count_1++;
		}
	}
	count_1 += count_moves(S.substr(middle, S.size() - middle), c + 1);

	// Now try the second-half as "== c".
	for(i = middle; i < S.size(); i++) {
		if(S[i] != c) {
			count_2++;
		}
	}
	count_2 += count_moves(S.substr(0, middle), c + 1);

	//if(count_1 < count_2) {
	//	return count_1 + count_moves(S, c + 1, middle, end);
	//} else if(count_1 > count_2) {
	//	return count_2 + count_moves(S, c + 1, start, middle - 1);
	//}
	//return min(count_1 + count_moves(S, c + 1, middle, end), count_2 + count_moves(S, c + 1, start, middle - 1));
	return min(count_1, count_2);
}

void testCase() {
	int N, i;
	string S;

	cin >> N;
	cin >> S;

	//cout << "string: " << S << " - " << S[0];

	cout << count_moves(S, 'a') << endl;
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
