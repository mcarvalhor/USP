#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <math.h>

using namespace std;

#define ERR 1e-7



double function(long p, long q, long r, long s, long t, long u, double x){
  return p*exp(-x) + q*sin(x) + r*cos(x) + s*tan(x) + t*x*x + u;
}

void solve(long p, long q, long r, long s, long t, long u){
	double x, max = 1, min = 0;
	if(function(p, q, r, s, t, u, max) * function(p, q, r, s, t, u, min) > 0){
		printf("No solution\n");
		return;
	}
	while(min + ERR < max){
		x = (max + min) / 2.0;
		if(function(p, q, r, s, t, u, min) * function(p, q, r, s, t, u, x) <= 0)
			max = x;
		else
			min = x;
	}
	printf("%.4lf\n", (max + min) / 2.0);
}

int main(int argc, char **argv) {
	long p, q, r, s, t, u;
	while(scanf("%ld %ld %ld %ld %ld %ld", &p, &q, &r, &s, &t, &u) == 6){
		solve(p, q, r, s, t, u);
	}
}
