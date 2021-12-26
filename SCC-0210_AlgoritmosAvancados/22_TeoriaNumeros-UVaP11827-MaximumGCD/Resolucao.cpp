#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;


long gcd(long A, long B){
	long Max;
	Max = (A < B) ? A : B;
	while(A % Max != 0 || B % Max != 0)	Max--;
	return Max;
}

int main(int argc, char **argv){
	long N, i, j, k, Aux1, Aux2, GCD;
	char Line[10000];
	vector<long> V;
	cin >> N;
	for(i = 0; i < N; i++){
		V = vector<long>();
		GCD = INT_MIN;
		cin >> Aux1;
		V.push_back(Aux1);
		fgets(Line, sizeof(Line) / sizeof(char), stdin);
		while(sscanf(Line, "%ld %[^\n]", &Aux1, Line) > 1){
			V.push_back(Aux1);
		}
		V.push_back(Aux1);
		Aux1 = V.size();
		for(j = 0; j < Aux1; j++){
			for(k = 0; k < Aux1; k++){
				if(j == k)	continue;
				Aux2 = gcd(V[j], V[k]);
				if(Aux2 > GCD)	GCD = Aux2;
			}
		}
		cout << GCD << '\n';
	}
	return EXIT_SUCCESS;
}



