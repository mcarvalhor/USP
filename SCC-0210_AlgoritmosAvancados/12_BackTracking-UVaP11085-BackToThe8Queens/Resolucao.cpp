#include <iostream>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>

using namespace std;


typedef struct {
    char Q[8];
    char O[8];
    int M;
} Game;

bool check(Game *G){
    int i, j;
    int DiagA, DiagB;
    for(i = 0; i < 8; i++){
        DiagA = G->Q[i] - i;
        DiagB = G->Q[i] + i;
        for(j = 0; j < i; j++){
            if(G->Q[j] == G->Q[i])  return false;
            if(G->Q[j] == G->Q[j]+DiagA)    return false;
            if(G->Q[j] == (-1*G->Q[j])+DiagB)   return false;
        }
        for(j = i+1; j < 8; j++){
            if(G->Q[j] == G->Q[i])  return false;
            if(G->Q[j] == G->Q[j]+DiagA)    return false;
            if(G->Q[j] == (-1*G->Q[j])+DiagB)   return false;
        }
    }
    return true;
}

int combine(Game *G, int pos){
    long i, min = INT_MAX, aux;
    if(pos >= 7){
        if(check(G)){
            return 0;
        }
        return min;
    }
    for(i = 0; i < 8; i++){
        G->Q[pos] = i;
        aux = abs(G->O[pos] - G->Q[pos]);
        aux = 1;
        aux += combine(G, pos+1);
        if(aux < min)   min = aux;
        //G->Q[pos] = G->O[pos];
    }
    return min;
}

int main(int argc, char **argv){
    unsigned int i, n = 1;
    int Q[8];
    Game Aux;
    while(scanf("%d %d %d %d %d %d %d %d", &Q[0], &Q[1], &Q[2], &Q[3], &Q[4], &Q[5], &Q[6], &Q[7]) == 8){
        memset(&Aux, 0, sizeof(Game));
        for(i = 0; i < 8; i++)
            Aux.O[i] = Q[i] - 1;
        cout << "Case " << n++ << ": " << combine(&Aux, 0) << "\n";
    }
}