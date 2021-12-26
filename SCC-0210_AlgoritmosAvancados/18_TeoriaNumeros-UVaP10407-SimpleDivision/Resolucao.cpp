#include <stdlib.h>
#include <iostream>
#include <limits.h>
#include <vector>
#include <cmath>

using namespace std;

int main(int argc, char **argv){
	int Aux, NA, i, j, Largest;
	while(1){
		Largest = INT_MAX;
		vector<int> Numbers = vector<int>();
		cin >> Aux;
		while(Aux != 0) {
			Numbers.push_back(Aux);
			cin >> Aux;
		}
		NA = Numbers.size();
		if(NA < 2)	break;
		for(i = 0; i < NA; i++){
			if(Numbers[i] < 0)	Numbers[i] = abs(Numbers[i]) + 1;
			if(Numbers[i] < Largest)	Largest = Numbers[i];
		}
		for(i = Largest; i >= 1; i--){
			if(Numbers[0] != i)	Aux = Numbers[0] % i;
			else	Aux = Numbers[1] % i;
			for(j = 1; j < NA; j++){
				if(Numbers[j] == i)	continue;
				if(Numbers[j] % i != Aux)	goto Next_Int;
			}
			break;
			Next_Int: ;
		}
		cout << i << '\n';
	}
}