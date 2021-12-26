#include <stdlib.h>
#include <stdio.h>

#include <iostream>
#include <map>



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



ll recursive_lcs(char *a, char *b, map< pair<char *, char *>, ll > &dp) {
	ll selectCount, passCount;
	if(*a == '\0' || *b == '\0') {
		return 0;
	}
	if(dp.count(make_pair(a, b)) > 0) {
		return dp[make_pair(a, b)];
	}
	if(*a == *b) {
		dp[make_pair(a, b)] = recursive_lcs(a + 1, b + 1, dp) + 1;
		return dp[make_pair(a, b)];
	}
	passCount = recursive_lcs(a + 1, b, dp);
	selectCount = recursive_lcs(a, b + 1, dp);
	if(selectCount >= passCount) {
		dp[make_pair(a, b)] = selectCount;
	} else {
		dp[make_pair(a, b)] = passCount;
	}
	return dp[make_pair(a, b)];
}

ll testCase(char *a, char *b) {
	map< pair<char *, char *>, ll > dp = map< pair<char *, char *>, ll >();
	return recursive_lcs(a, b, dp);
}

int main(int argc, char **argv) {
	char a[1001], b[1001];

	while(gets(a)) {
		gets(b);
		cout << testCase(a, b) << endl;
	}

	return EXIT_SUCCESS;
}