#include <stdlib.h>
#include <limits.h>
#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

using namespace std;

typedef struct {
	long pos;
	long aux;
	char type;
} ACTION;

enum { FUEL_CONSUMPTION, LEAK, GAS_STATION, MECHANIC, GOAL };




double solve(vector<ACTION> &actlist){
	long normal_consumption, leaked_consumption, last_pos;
	double min = LONG_MIN, current;
	vector<ACTION>::iterator it = actlist.begin();
	leaked_consumption = last_pos = 0;
	normal_consumption = it->aux;
	for(current = 0, it++; it != actlist.end(); it++){
		current += (it->pos - last_pos) * ((normal_consumption/100.0) + leaked_consumption);
		last_pos = it->pos;
		switch(it->type){
			case FUEL_CONSUMPTION:
				normal_consumption = it->aux;
			break;
			case LEAK:
				leaked_consumption++;
			break;
			case MECHANIC:
				leaked_consumption = 0;
			break;
			case GAS_STATION:
				if(current > min)	min = current;
				current = 0;
			break;
			default: ;
		}
	}
	if(current > min)	min = current;
	return min;
}



int main(int argc, char **argv) {
	string aux;
	ACTION a;
	do {
		a.type = FUEL_CONSUMPTION;
		cin >> a.pos;
		cin >> aux >> aux;
		cin >> a.aux;
		if(a.aux < 1)	continue;
		vector<ACTION> v;
		v.push_back(a);
		do {
			cin >> a.pos >> aux;
			if(aux == "Leak"){
				a.type = LEAK;
			}else if(aux == "Fuel"){
				a.type = FUEL_CONSUMPTION;
				cin >> aux >> a.aux;
			}else if(aux == "Gas"){
				a.type = GAS_STATION;
				cin >> aux;
			}else if(aux == "Mechanic"){
				a.type = MECHANIC;
			}else{
				a.type = GOAL;
			}
			v.push_back(a);
		} while(a.type != GOAL);
		printf("%.3lf\n", solve(v));
	} while(a.aux > 0);
}
