#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <iostream>
#include <algorithm>
#include <unordered_map>
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



/*
vector< vector<ll> > findSubsets(vector<ll> &set, ll min_size, ll max_size) {
	ll N, count, i, j;

	vector< vector<ll> > subsets = vector< vector<ll> >();
	vector<ll> subset = vector<ll>();
	N = set.size();
	count = pow(2, N);

	// Generate all possible subsets.
	for(i = 0; i < count; i++) {
		subset.clear();
		for(j = 0; j < N; j++) {
			if((i & (1 << j)) != 0) {
				subset.push_back(set[j]);
			}
		}
		subsets.push_back(subset);
	}

	// Remove subsets that don't respect minimum and maximum length.
	for(auto it = subsets.begin(); it != subsets.end(); it++) {
		if((*it).size() >= min_size && (*it).size() <= max_size) {
			continue;
		}
		subsets.erase(it--);
	}

	return subsets;
}
*/

void recursive_sum(ll position, ll sPosition, ll *totalsum, vector< unordered_map<ll, bool> > &dp, unordered_map<ll, bool> &sums, vector<ll> &set) {
	if(sums[sPosition] == false) {
		sums[sPosition] = true;
		*totalsum += sPosition;
	}
	if(position < 0 || position >= set.size()) {
		return;
	}
	if(dp[position][sPosition]) {
		return;
	}
	recursive_sum(position + 1, sPosition, totalsum, dp, sums, set);
	recursive_sum(position + 1, sPosition + set[position], totalsum, dp, sums, set);
	dp[position][sPosition] = true;
}

ll testCase() {
	ll N, i, aux, totalsum;

	vector<ll> set = vector<ll>();
	vector< unordered_map<ll, bool> > dp = vector< unordered_map<ll, bool> >();
	unordered_map<ll, bool> sums = unordered_map<ll, bool>();
	cin >> N;
	for(i = 0; i < N; i++) {
		cin >> aux;
		set.push_back(aux);
		dp.push_back( unordered_map<ll, bool>() );
	}
	
	totalsum = 0;
	recursive_sum(0, 0, &totalsum, dp, sums, set);

	return totalsum;
}

int main(int argc, char **argv) {
	ll T;

	cin >> T;
	for(; T > 0; T--) {
		cout << testCase() << endl;
	}

	return EXIT_SUCCESS;
}
