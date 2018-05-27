#include <iostream>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <queue>
#include <string.h>

using namespace std;


typedef struct __node {
	int Curr[4];
	int Dep;
	int Distance;
	bool operator<(const __node &a) const{
		return Dep+Distance > a.Dep+a.Distance;
	}
} V_NODE;

int distance(int *A, int *B){
	int i, sum;
	for(i = sum = 0; i < 4; i++)
		sum += abs(A[i]	 - B[i]); // * pow(10, i);
	return sum;
}

bool is_forb(int *A, vector<int*> &Forb_List){
	int i, j, n;
	n = Forb_List.size();
	for(i = 0; i < n; i++){
		for(j = 0; j < 4; j++)
			if(Forb_List[i][j] != A[j])	break;
		if(j >= 4)	return true;
	}
	return false;
}

int main(int argc, char **argv){
	int i, j, k, N, Objective[4], Aux[4], NForb;
	int *AuxVet;
	V_NODE AuxNode;
	V_NODE ProcessingNode;
	cin >> N;
	for(i = 0; i < N; i++){
		vector<int*> Forb_List = vector<int*>();
		priority_queue<V_NODE> Nodes = priority_queue<V_NODE>();
		cin >> AuxNode.Curr[0] >> AuxNode.Curr[1] >> AuxNode.Curr[2] >> AuxNode.Curr[3];
		cin >> Objective[0] >> Objective[1] >> Objective[2] >> Objective[3];
		cin >> NForb;
		for(j = 0; j < NForb; j++){
			cin >> Aux[0] >> Aux[1] >> Aux[2] >> Aux[3];
			if(distance(Aux, Objective) == 0 || distance(Aux, AuxNode.Curr) == 0){
				for(k = j+1; k < NForb; k++){
					cin >> Aux[0] >> Aux[1] >> Aux[2] >> Aux[3];
				}
				cout << "-1\n"; // Objetivo ou Inicio está na lista de proibidos.
				goto continue_loop;
			}
			AuxVet = (int *)malloc(sizeof(int) * 4);
			memcpy(AuxVet, &Aux, sizeof(int) * 4);
			Forb_List.push_back(AuxVet);
		}
		AuxNode.Dep = 0;
		AuxNode.Distance = distance(AuxNode.Curr, Objective);
		Nodes.push(AuxNode);
		while(Nodes.size() > 0){
			// Retirar node[0] + calcular nodes filhos + inseri-los
			ProcessingNode = Nodes.top();
			if(distance(ProcessingNode.Curr, Objective) == 0){
				cout << ProcessingNode.Dep << "\n";
				goto continue_loop;
			}
			Nodes.pop();
			AuxNode.Dep = ProcessingNode.Dep + 1;
			memcpy(AuxNode.Curr, ProcessingNode.Curr, sizeof(int) * 4);
			for(j = 0; j < 4; j++){
				if(++AuxNode.Curr[j] < 10 && !is_forb(AuxNode.Curr, Forb_List)){
					AuxNode.Distance = distance(AuxNode.Curr, Objective);
					Nodes.push(AuxNode);
				}
				AuxNode.Curr[j] = ProcessingNode.Curr[j];
				if(--AuxNode.Curr[j] >= 0 && !is_forb(AuxNode.Curr, Forb_List)){
					AuxNode.Distance = distance(AuxNode.Curr, Objective);
					Nodes.push(AuxNode);
				}
				AuxNode.Curr[j] = ProcessingNode.Curr[j];
			}
		}
		cout << "-1\n"; // Não possível.
		continue_loop: ;
	}
	return EXIT_SUCCESS;
}