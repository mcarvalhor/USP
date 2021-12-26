#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <vector>



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
typedef long long ll;



void testCase() {
	ll strLen, patternLen;
	char str[1000001], pattern[1000001], *itt;

	cin >> str >> pattern;
	strLen = strlen(str);
	patternLen = strlen(pattern);
	vector<ll> found = vector<ll>();

	for(itt = str; itt <= str + strLen - patternLen; itt++) {
		if(memcmp(itt, pattern, patternLen) == 0) {
			found.push_back((ll) (itt - str));
		}
	}

	if(found.size() < 1) {
		cout << "Not Found" << endl;
		return;
	}

	cout << found.size() << endl;
	for(auto it = found.begin(); it != found.end(); it++) {
		cout << (*it + 1) << ' ';
	}
	cout << endl;
}



int main(int argc, char **argv) {
	ll T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
		cout << endl;
	}

	return EXIT_SUCCESS;
}


