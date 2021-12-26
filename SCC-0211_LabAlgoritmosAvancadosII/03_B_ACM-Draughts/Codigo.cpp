#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>



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

int captures[4][2] = {
	{-1, -1},
	{-1, 1},
	{1, -1},
	{1, 1}
};

int moves[4][2] = {
	{-2, -2},
	{-2, 2},
	{2, -2},
	{2, 2}
};



using namespace std;



int backtrack(int x, int y, int originalX, int originalY, vector< vector< tuple<char, bool> > > &table) {
	int i, N, greatest, tmp;

	N = table.size();
	greatest = 0;
	for(i = 0; i < 4; i++) {
		if(x + moves[i][0] >= N || x + moves[i][0] < 0 || y + moves[i][1] >= N || y + moves[i][1] < 0) {
			continue;
		}
		if(get<0>(table[x + captures[i][0]][y + captures[i][1]]) != 'B') {
			continue;
		}
		if((x + moves[i][0] != originalX || y + moves[i][1] != originalY) && (get<0>(table[x + moves[i][0]][y + moves[i][1]]) != '#')) {
			continue;
		}
		if(get<1>(table[x + captures[i][0]][y + captures[i][1]]) == true) {
			continue;
		}
		get<1>(table[x + captures[i][0]][y + captures[i][1]]) = true;
		tmp = 1 + backtrack(x + moves[i][0], y + moves[i][1], originalX, originalY, table);
		get<1>(table[x + captures[i][0]][y + captures[i][1]]) = false;
		if(tmp > greatest) {
			greatest = tmp;
		}
	}

	return greatest;
}

void testCase() {
	int i, j, N, greatest, tmp;
	string aux;

	N = 10; // Constant!

	vector< vector< tuple<char, bool> > > table;
	for(i = 0; i < N; i++) {
		table.push_back(vector< tuple<char, bool> >());
		cin >> aux;
		for(j = 0; j < N; j++) {
			table[i].push_back( tuple<char, bool>(aux[j], false) );
		}
	}

	greatest = 0;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			if(get<0>(table[i][j]) != 'W') {
				continue;
			}
			tmp = backtrack(i, j, i, j, table);
			if(tmp > greatest) {
				greatest = tmp;
			}
		}
	}

	cout << greatest << endl;
}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {
		testCase();
	}

	return EXIT_SUCCESS;
}
