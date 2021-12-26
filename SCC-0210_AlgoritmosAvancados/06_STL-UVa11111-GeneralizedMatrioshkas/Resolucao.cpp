#include <stack>
#include <iostream>
#include <algorithm>
#include <string.h>
#include <stdio.h>
using namespace std;


class Main {

    private: enum __Matrioska_LastStackAction_t { Popped = 0, Pushed, Unknown };

    public: int  Matrioska(FILE *FStream){
        /*
        * Esta função verifica se em 'FStream' há uma sequência que descreve uma "Matrioska" aguardando para ser lida.
        *
        * Ela retorna o número de Matrioskas possíveis de ser criadas com a sequência, ou -1 em caso de erros de empilhamento, e -2 em caso de erros de chegar ao fim sem terminar de fato a sequência.
        */
        stack<int> DoolStack;
        int N=0, R, Aux = 0;
        enum __Matrioska_LastStackAction_t Last=Popped; // Popped
        fscanf(FStream,"%d",&R);
        if(R==0){
            return 0; // Começou finalizando.
        }else if(R>0){ //printf("ERRO EM 1\n");
            return -2; // O primeiro valor da sequência já indica que não é uma Matroska.
        }
        DoolStack.push(123456);
        while(DoolStack.size() > 0){
            //printf("R = %d\n", R);
            if(R<0){
                if(DoolStack.size()>0 && Last==Pushed && R<DoolStack.top()){//printf("ERRO EM 2\n");
                    return -2;
                }
                if(DoolStack.size()>0 && Last==Popped && Aux != 0 && Aux+R <= DoolStack.top()){
                    return -2;
                }
                Aux = 0;
                DoolStack.push(R);
                if(Last==Popped){
                    N++;
                }
                Last=Pushed;
            }else if(R>0){
                if(DoolStack.size()<=1 || (R*-1)!=DoolStack.top()){ //printf("ERRO EM 3: size = %ld, top = %d\n", DoolStack.size(), DoolStack.top());
                    return -2;
                }
                Aux = DoolStack.top();
                DoolStack.pop();
                Last=Popped;
            }
            if(DoolStack.size() == 1)   break;
            fscanf(FStream,"%d",&R);
        }
        if(DoolStack.size()!=1 || DoolStack.top() != 123456){
            return -1;
        }
        return N; // Sucesso na sequência.
    }

    public: Main(){

        //int auxn;
        char AuxStr[1000];

        while(scanf("%[^\n]%*c", AuxStr) > 0){
            //printf("%s\n", AuxStr);
            FILE *Aux = fmemopen(AuxStr, strlen(AuxStr), "r");
            if( Matrioska(Aux) > 0 ){
                printf(":-) Matrioshka!\n");
            }else{
                printf(":-( Try again.\n");
            }
        }

    }

};

int main(){

    Main a;

    return 0;
}