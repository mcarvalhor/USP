#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
    int Soma = 0, Aux1, Aux2;
    while(scanf("%d %d", &Aux1, &Aux2) == 2){
        Soma += Aux1 + Aux2;
    }
    printf("%d\n", Soma);
    return  EXIT_SUCCESS;
}
