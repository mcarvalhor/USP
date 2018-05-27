#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


using namespace std;




typedef struct {
	int G[8][8];
	int Q[8];
	vector< vector<int> > solutions;
} Game;



void insert_solution(Game *G){
	unsigned int i, j, nG, nS;
	vector<int> S = vector<int>();
	for(i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			if(G->G[j][i] < 0){
				S.push_back(j);
				break;
			}
		}
	}
	if(S.size() != 8)	return;
	nG = G->solutions.size();
	for(i = 0; i < nG; i++){
		nS = G->solutions[i].size();
		if(nS != S.size())	continue;
		for(j = 0; j < nS; j++){
			if(S[j] != G->solutions[i][j])	goto end_check;
		}
		return;
		end_check: ;
	}
	G->solutions.push_back(S);
}


bool mark(int Row, int Column, int id, Game *G){
	int i, DiagA, DiagB, Aux1, Aux2;
	if(Row < 0 || Row >= 8 || Column < 0 || Column >= 8)	return false;
	if(G->G[Row][Column] != 0)	return false;
	G->G[Row][Column] = (G->G[Row][Column] | id) | 0b10000000000000000000000000000000;
	DiagA = Column - Row;
	DiagB = Column + Row;
	for(i = 0; i < 8; i++){
		G->G[Row][i] = G->G[Row][i] | id;
		G->G[i][Column] = G->G[i][Column] | id;
		Aux1 = i+DiagA;
		Aux2 = (-1*i)+DiagB;
		if(Aux1 >= 0 && Aux1 < 8)	G->G[i][Aux1] = G->G[i][Aux1] | id;
		if(Aux2 >= 0 && Aux2 < 8)	G->G[i][Aux2] = G->G[i][Aux2] | id;
	}
	return true;
}


bool unmark(int Row, int Column, int id, Game *G){
	int i, DiagA, DiagB, Aux1, Aux2;
	if(Row < 0 || Row >= 8 || Column < 0 || Column >= 8)	return false;
	G->G[Row][Column] = (G->G[Row][Column] & ~id) & 0b01111111111111111111111111111111;
	DiagA = Column - Row;
	DiagB = Column + Row;
	for(i = 0; i < 8; i++){
		G->G[Row][i] = G->G[Row][i] & ~id;
		G->G[i][Column] = G->G[i][Column] & ~id;
		Aux1 = i+DiagA;
		Aux2 = (-1*i)+DiagB;
		if(Aux1 >= 0 && Aux1 < 8)	G->G[i][Aux1] = G->G[i][Aux1] & ~id;
		if(Aux2 >= 0 && Aux2 < 8)	G->G[i][Aux2] = G->G[i][Aux2] & ~id;
	}
	return true;
}


void combine(int Row, int Column, int id, Game *G){
	int i;
	if(id >= 8){
		insert_solution(G);
		return;
	}
	if(mark(Row, Column, pow(2, id), G)){
		for(i = 0; i < 8; i++){
			combine(i, Column + 1, id + 1, G);
			combine(i, Column + 2, id + 1, G);
		}
		unmark(Row, Column, pow(2, id), G);
	}
}

void print_solutions(Game *G){
	int i, j, nA, nB;
	nA = G->solutions.size();
	cout << "SOLN	COLUMN\n" << " #\t\t1 2 3 4 5 6 7 8\n\n";
	for(i = 0; i < nA; i++){
		printf("%2d     ", i+1);
		nB = G->solutions[i].size();
		for(j = 0; j < nB; j++){
			cout << " " << G->solutions[i][j] + 1;
		}
		cout << "\n";
	}
}


int main(int argc, char **argv){
	int N, i, Row, j, Column;
	Game Aux;
	cin >> N;
	for(i = 0; i < N; i++){
		if(i)	cout << "\n";
		cin >> Row >> Column;
		Row--;
		Column--;
		memset(&Aux, 0, sizeof(Game));
		Aux.solutions = vector< vector<int> >();
		mark(Row, Column, 1, &Aux);
		for(j = 0; j < 8; j++){
			combine(j, 0, 1, &Aux);
		}
		for(j = 0; j < 8; j++){
			combine(j, Column + 1, 1, &Aux);
		}
		unmark(Row, Column, 1, &Aux);
		print_solutions(&Aux);
	}
	return EXIT_SUCCESS;
}







