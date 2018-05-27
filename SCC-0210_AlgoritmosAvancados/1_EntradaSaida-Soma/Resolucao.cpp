#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
    int Soma = 0, Aux1;
    scanf("%*d");
    while(scanf("%d", &Aux1) == 1){
        Soma += Aux1;
    }
    printf("%d\n", Soma);
    return  EXIT_SUCCESS;
}