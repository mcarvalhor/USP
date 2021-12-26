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

int moves[8][2] = {
	{-1, 0},
	{1, 0},
	{0, -1},
	{0, 1},
	{1, -1},
	{-1, 1},
	{1, 1},
	{-1, -1}
};


using namespace std;

bool sort_cmp(string &a, string &b) {
	if(a.size() == b.size()) {
		return a < b;
	} else if(a.size() < b.size()) {
		return true;
	} else {
		return false;
	}
}

vector<string> backtrack(int x, int y, string beginning, char lastChar, vector< vector< tuple<char, bool> > > &table) {
	char currentChar, nextChar;
	vector<string> found;
	vector<string> aux;
	int i;
	if(x >= table.size() || y >= table.size() || x < 0 || y < 0) {
		return found;
	}
	if( get<1>(table[x][y]) != false ) {
		return found;
	}

	currentChar = get<0>(table[x][y]);
	if(currentChar <= lastChar) {
		return found;
	}

	found.push_back(beginning + currentChar);
	get<1>(table[x][y]) = true;

	for(i = 0; i < 8; i++) {
		aux = backtrack(x + moves[i][0], y + moves[i][1], beginning + currentChar, currentChar, table);
		found.insert(found.end(), aux.begin(), aux.end());
	}

	get<1>(table[x][y]) = false;
	return found;
}

void testCase() {
	int i, j, N;

	cin >> N;

	vector< vector< tuple<char, bool> > > table;
	vector< vector< vector<string> > > foundWords;
	string aux;
	for(i = 0; i < N; i++) {
		cin >> aux;
		table.push_back(vector< tuple<char, bool> >());
		foundWords.push_back(vector< vector<string> >());
		for(j = 0; j < N; j++) {
			table[i].push_back(tuple<char, bool>(aux[j], false));
			foundWords[i].push_back(vector<string>());
		}
	}

	vector<string> found;
	vector<string> auxF;
	for(i = 0; i < N; i++) {
		for(j = 0; j < N; j++) {
			auxF = backtrack(i, j, "", 0, table);
			found.insert(found.end(), auxF.begin(), auxF.end());
		}
	}

	unordered_map<string, bool> foundMap;

	for(vector<string>::iterator it = found.begin(); it != found.end(); it++) {
		if((*it).size() < 3 || (*it).size() > N*N) {
			continue;
		}
		foundMap[*it] = true;
	}

	found.clear();
	for(unordered_map<string, bool>::iterator it = foundMap.begin(); it != foundMap.end(); it++) {
		found.push_back(it->first);
	}

	foundMap.clear();
	for(vector<string>::iterator it = found.begin(); it != found.end(); it++) {
		if((*it).size() < 3 || (*it).size() > N*N) {
			continue;
		}
		foundMap[*it] = true;
	}

	sort(found.begin(), found.end(), sort_cmp);

	for(vector<string>::iterator it = found.begin(); it != found.end(); it++) {
		cout << *it << endl;
	}

}

int main(int argc, char **argv) {
	int T;

	cin >> T;
	for(; T > 0; T--) {

		testCase();
		if(T > 1) {
			cout << endl;
		}

	}

	return EXIT_SUCCESS;
}
