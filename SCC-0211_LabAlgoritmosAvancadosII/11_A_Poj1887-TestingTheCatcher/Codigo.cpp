#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <iostream>
#include <algorithm>
#include <map>
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



ll recursive_count_max(ll position, ll top, map<pair<ll, ll>, ll> &pd, vector<ll> &missiles) {
	ll counter1, counter2;
	if(position < 0 || position >= missiles.size()) {
		return 0;
	}

	if(pd.count(make_pair(position, top)) > 0) {
		return pd[make_pair(position, top)];
	}

	counter1 = recursive_count_max(position + 1, top, pd, missiles);
	counter2 = -1;
	if(missiles[position] <= top) {
		counter2 = recursive_count_max(position + 1, missiles[position], pd, missiles) + 1;
	}

	if(counter1 >= counter2) {
		pd[make_pair(position, top)] = counter1;
		return counter1;
	}
	pd[make_pair(position, top)] = counter2;
	return counter2;
}

ll testCase() {
	ll i, aux;

	vector<ll> missiles = vector<ll>();
	map<pair<ll, ll>, ll> pd = map< pair<ll, ll>, ll >();
	cin >> aux;
	while(aux >= 0) {
		missiles.push_back(aux);
		cin >> aux;
	}

	if(missiles.size() < 1) {
		return -1;
	}

	return recursive_count_max(0, INT_MAX, pd, missiles);
}

int main(int argc, char **argv) {
	ll intercepted, counter;

	counter = 1;
	while((intercepted = testCase()) >= 0) {
		if(counter > 1) {
			cout << endl;
		}
		cout << "Test #" << counter << ":" << endl;
		cout << "  maximum possible interceptions: " << intercepted << endl;
		counter++;
	}

	return EXIT_SUCCESS;
}
