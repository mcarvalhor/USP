#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

vector<int> crivo = vector<int>(100000);
long crivoCount = 0;

void CalcularCrivo(){
	int i, j, N, m[crivo.size() * 10];
	memset(m, 0, sizeof(m));
	N = crivo.size();
    for(i = 2; i < N; i++)
		if(m[i] == 0) {
			crivo[crivoCount++] = i;
			for(j = i + i; j < N; j += i)
				m[j] = 1;
		}
}

bool Divides(long N, long Div){
	if(Div == 0)	return false;
	if(N >= Div || Div == 1)	return true;
	long tmp, cnt2;
	int i, cnt;
	for(i = 0; i < crivoCount && crivo[i]*crivo[i] <= Div; i++) {
        if(Div % crivo[i] == 0) {
            cnt = 0;
            while(Div % crivo[i] == 0){
                cnt++;
				Div /= crivo[i];
			}
            tmp = crivo[i];
			cnt2 = 0;
            while(tmp <= N) {
                cnt2 += N/tmp;
                tmp *= crivo[i];
            }
            if(cnt2 < cnt)  return false;
        }
    }
    if(Div != 1 && N < Div)	return false;
	return true;
}

int main(int argc, char **argv){
	long N, Div;
	CalcularCrivo();
	while(scanf("%ld %ld", &N, &Div) == 2){
		if(Divides(N, Div))
			cout << Div << " divides " << N << "!\n";
		else
			cout << Div << " does not divide " << N << "!\n";
	}
	return EXIT_SUCCESS;
}


