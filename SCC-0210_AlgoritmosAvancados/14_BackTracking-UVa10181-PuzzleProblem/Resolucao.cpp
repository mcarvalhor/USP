#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;


typedef struct __node {
	int blki, blkj;
	int Org[4][4];
	int Diff;
	vector<char> Path;
	bool operator<(const __node &a) const{
		return Path.size()+Diff > a.Path.size()+a.Diff;
	}
} V_NODE;

int difference(int A[][4]){
	int i, j, sum;
	int Aux[4][4] = {
		{1, 2, 3, 4},
		{5, 6, 7, 8},
		{9, 10, 11, 12},
		{13, 14, 15, 0}
	};
	for(i = sum = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			sum += abs(A[i][j] - Aux[i][j]); // * pow(10, i);
	return sum;
}

void swap(int *A, int *B){
	int Aux = *A;
	*A = *B;
	*B = Aux;
}

vector<char> cpy_vec(vector<char> A){
	vector<char> Aux = vector<char>();
	int i, n;
	for(n = A.size(), i = 0; i < n; i++){
		Aux.push_back(A[i]);
	}
	return Aux;
}

void prn_vec(vector<char> A){
	int i, n;
	for(n = A.size(), i = 0; i < n; i++){
		cout << A[i];
	}
	cout << "\n";
}

int main(int argc, char **argv){
	int i, j, k, N;
	V_NODE AuxNode;
	V_NODE ProcessingNode;
	cin >> N;
	for(k = 0; k < N; k++){
		priority_queue<V_NODE> Nodes = priority_queue<V_NODE>();
		AuxNode.Path = vector<char>();
		for(i = 0; i < 4; i++){
			for(j = 0; j < 4; j++){
				cin >> AuxNode.Org[i][j];
				if(AuxNode.Org[i][j] == 0){
					AuxNode.blki = i;
					AuxNode.blkj = j;
				}
			}
		}
		AuxNode.Diff = difference(AuxNode.Org);
		Nodes.push(AuxNode);
		while(Nodes.size() > 0){
			ProcessingNode = Nodes.top();
			if(difference(ProcessingNode.Org) == 0){
				prn_vec(ProcessingNode.Path);
				goto end_loop;
			}
			if(ProcessingNode.Path.size() > 20){
				break;
			}
			Nodes.pop();
			memcpy(AuxNode.Org, ProcessingNode.Org, sizeof(int) * 4 * 4);
			AuxNode.blki = ProcessingNode.blki;
			AuxNode.blkj = ProcessingNode.blkj;
			if(AuxNode.blki + 1 < 4){
				AuxNode.blki++;
				AuxNode.Path = cpy_vec(ProcessingNode.Path);
				AuxNode.Path.push_back('D');
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki - 1][AuxNode.blkj]);
				AuxNode.Diff = difference(AuxNode.Org);
				Nodes.push(AuxNode);
				AuxNode.blki--;
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki + 1][AuxNode.blkj]);
			}
			if(AuxNode.blki - 1 >= 0){
				AuxNode.blki--;
				AuxNode.Path = cpy_vec(ProcessingNode.Path);
				AuxNode.Path.push_back('U');
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki + 1][AuxNode.blkj]);
				AuxNode.Diff = difference(AuxNode.Org);
				Nodes.push(AuxNode);
				AuxNode.blki++;
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki - 1][AuxNode.blkj]);
			}
			if(AuxNode.blkj + 1 < 4){
				AuxNode.blkj++;
				AuxNode.Path = cpy_vec(ProcessingNode.Path);
				AuxNode.Path.push_back('R');
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki][AuxNode.blkj - 1]);
				AuxNode.Diff = difference(AuxNode.Org);
				Nodes.push(AuxNode);
				AuxNode.blkj--;
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki][AuxNode.blkj + 1]);
			}
			if(AuxNode.blkj - 1 >= 0){
				AuxNode.blkj--;
				AuxNode.Path = cpy_vec(ProcessingNode.Path);
				AuxNode.Path.push_back('L');
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki][AuxNode.blkj + 1]);
				AuxNode.Diff = difference(AuxNode.Org);
				Nodes.push(AuxNode);
				AuxNode.blkj++;
				swap(&AuxNode.Org[AuxNode.blki][AuxNode.blkj], &AuxNode.Org[AuxNode.blki][AuxNode.blkj - 1]);
			}
		}
		cout << "This puzzle is not solvable.\n";
		end_loop: ;
	}
	return EXIT_SUCCESS;
}