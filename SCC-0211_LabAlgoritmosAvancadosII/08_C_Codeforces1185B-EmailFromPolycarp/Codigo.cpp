#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>



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
	string s, t;
	char lastCh;
	ll i, j;
	bool ok;

	cin >> s >> t;
	if(s[0] != t[0]) {
		cout << "NO" << endl;
		return;
	}
	lastCh = t[0];
	i = 1;
	for(j = 1; j < s.size(); j++) {
		ok = false;
		for(; i < t.size(); i++) {
			if(s[j] == t[i]) {
				lastCh = t[i];
				ok = true;
				i++;
				break;
			}
			if(t[i] != lastCh) {
				cout << "NO" << endl;
				return;
			}
		}
		if(!ok) {
			cout << "NO" << endl;
			return;
		}
	}
	for(; i < t.size(); i++) {
		if(t[i] != lastCh) {
			cout << "NO" << endl;
			return;
		}
	}
	cout << "YES" << endl;
}

int main(int argc, char **argv) {
	ll T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
