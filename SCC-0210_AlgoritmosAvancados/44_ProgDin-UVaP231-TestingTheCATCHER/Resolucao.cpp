#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <vector>

using namespace std;


int main(int argc, char **argv) {
	vector< vector<long> > missiles;
	long i, j, k, Aux, n, nmissiles, counter;

	missiles = vector< vector<long> >();
	missiles.push_back(vector<long>());
	cin >> Aux;
	n = 0;
	do {
		if(Aux < 0) {
			missiles.push_back(vector<long>());
			n++;
			continue;
		}
		missiles[n].push_back(Aux);
	} while(cin >> Aux);
	n--;

	for(i = 0; i < n; i++){
		if(i)
			cout << '\n';
		cout << "Test #" << i + 1 << ":\nmaximum possible interceptions: ";
		nmissiles = missiles[i].size();
		k = -1;
		for(j = counter = 0; j < nmissiles; j++){
			if(k < 0){
				counter++;
				k = 0;
			} else if(missiles[i][j] <= missiles[i][k]) {
				counter++;
				k++;
			} else {
				k--;
			}
		}
		cout << counter << "\n";
	}

	return EXIT_SUCCESS;
}


