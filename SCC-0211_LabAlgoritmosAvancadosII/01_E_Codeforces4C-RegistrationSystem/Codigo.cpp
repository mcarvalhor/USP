#include <stdlib.h>

#include <unordered_map>
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

int main(int argc, char **argv) {
	string name, current_name;
	int N;

	unordered_map<string, int> found_count;
	found_count = unordered_map<string, int>(); // Criar.

	scanf("%d", &N);
	for(; N > 0; N--) {
		cin >> name;
		if(found_count[name] == 0) {
			cout << "OK" << endl;
		} else {
			cout << name << found_count[name] << endl;
		}
		found_count[name]++;
	}
	
	return EXIT_SUCCESS;
}
