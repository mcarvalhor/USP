#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <cmath>

using namespace std;

long N_Divisors(long N){
	long i, Max, Counter;
	Max = sqrt(N);
	for(i = 2, Counter = 0; i <= Max; i++){
		if(N % i == 0){
			if(N / i == i)	Counter++;
			else	Counter += 2;
		}
	}
	return Counter + 2; // 1 e ele mesmo
}

int main(int argc, char **argv){
	long i, j, N, Start, End, LargestId, LargestN, Aux;
	cin >> N;
	for(i = 0; i < N; i++){
		cin >> Start >> End;
		LargestN = INT_MIN;
		for(j = Start; j <= End; j++){
			Aux = N_Divisors(j);
			if(Aux > LargestN){
				LargestId = j;
				LargestN = Aux;
			}
		}
		cout << "Between " << Start << " and " << End << ", " << LargestId << " has a maximum of " << LargestN << " divisors.\n";
	}
	return EXIT_SUCCESS;
}


