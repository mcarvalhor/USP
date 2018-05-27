#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <cmath>

using namespace std;


int abtoi(char *Str){
	int counter, N, i, Aux;
	N = strlen(Str);
	for(i = N - 1, counter = Aux = 0; i >= 0 && counter < 30; i--, counter++){
		if(Str[i] == '1'){
			Aux += pow(2, counter);
		}else if(Str[i] != '0'){
			counter--;
		}
	}
	return Aux;
}

int main(int argc, char **argv){
	char Digit[100005], R;
	int N = 131071, Aux;
	while(scanf("%[^#]%c", Digit, &R) == 2){
		Aux = abtoi(Digit);
		if(Aux % N == 0){
			cout << "YES\n";
		}else{
			cout << "NO\n";
		}
	}
	return EXIT_SUCCESS;
}