#include <stdlib.h>
#include <iostream>
#include <map>

using namespace std;



void count(long *cents, long n, long limit, map<long, long> &counter) {
	long i, j;
	for(i = 0; i < n; i++){
		for(j = cents[i]; j < limit; j++){
			counter[j] += counter[j - cents[i]];
		}
	}
}


int main(int argc, char **argv) {
	long cents[] = { 50, 25, 10, 5, 1 };
	map<long, long> counter;
	long n, value;
	n = 5;

	counter[0] = 1;
	count(cents, n, 400000, counter);

	while(cin >> value) {
		cout << counter[value] << '\n';
	}
	return EXIT_SUCCESS;
}

