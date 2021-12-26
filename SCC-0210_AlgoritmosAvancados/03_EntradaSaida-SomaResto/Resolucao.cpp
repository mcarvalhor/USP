#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
    int N, i, Aux, Aux2, Scanned;
    char Line[10000];
    char O[100], Op;
    scanf("%d", &N);
    for(i = 0; i < N; i++){
        scanf("%s %[^\n]s", O, Line);
        Op = O[0];
        switch(Op){
            case 'S':
                Aux = 0;
                while((Aux2=sscanf(Line, "%d %[^\n]s", &Scanned, Line)) == 2 || Aux2 == 1){
                    if(Aux2 == 1)   Line[0] = '\0';
                    Aux += Scanned;
                }
                printf("T %d\n", Aux);
            break;
            case 'R':
                sscanf(Line, "%d %[^\n]s", &Aux, Line);
                while((Aux2=sscanf(Line, "%d %[^\n]s", &Scanned, Line)) == 2 || Aux2 == 1){
                    if(Aux2 == 1)   Line[0] = '\0';
                    Aux -= Scanned;
                }
                printf("T %d\n", Aux);
            break;
            default:
                printf("E\n");
        }
    }
    return  EXIT_SUCCESS;
}