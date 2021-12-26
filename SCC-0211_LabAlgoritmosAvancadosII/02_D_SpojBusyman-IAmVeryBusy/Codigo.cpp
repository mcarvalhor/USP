#include <stdlib.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <tuple>



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

bool tuple_cmp(tuple<int, int> a, tuple<int, int> b) {
	/*
	if (get<0>(a) < get<0>(b)) {
		return true;
	}
	if(get<0>(a) == get<0>(b) && get<1>(a) < get<1>(b)) {
		return true;
	}
	*/
	if (get<1>(a) < get<1>(b)) {
		return true;
	}
	return false;
}

int main(int argc, char **argv) {
	int T, N, i, start, end, current, count;
	
	vector<tuple<int, int>> data;
	cin >> T;
	for(; T > 0; T--) {

		cin >> N;
		data.clear();
		for(i = 0; i < N; i++) {
			cin >> start >> end;
			data.push_back(make_tuple(start, end));
		}

		count = 0;
		current = 0;
		sort(data.begin(), data.end(), tuple_cmp);
		for(i = 0; i < N; i++) {
			if(get<0>(data[i]) < current) {
				continue;
			}
			current = get<1>(data[i]);
			count++;
		}
		cout << count << endl;


	}

	return EXIT_SUCCESS;
}
