#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <queue.h>
#include <heap.h>

int compare_integer(void *PA, void *PB){
	int *A=PA, *B=PB;
	return (*B)-(*A);
}

int main(int argc, char **argv){
	STACK *Data_S;
	QUEUE *Data_Q;
	HEAP *Data_H;
	int i,N,Action,Element,Aux;
	char WhatIsIt;
	scanf("%d", &N);
	while(N!=0){
		Data_S=S_New(sizeof(int));
		Data_Q=Q_New(sizeof(int));
		Data_H=H_New(sizeof(int), compare_integer);
		WhatIsIt=7;
		for(i=0;i<N;i++){
			scanf("%d %d", &Action,&Element);
			if(Action==1){ // Adicionar
				if(WhatIsIt&1)	S_Push(&Element,Data_S);
				if(WhatIsIt&2)	Q_Add(&Element,Data_Q);
				if(WhatIsIt&4)	H_Add(&Element,Data_H);
			}else{ // Remover
				if(WhatIsIt&1){
					S_Pop(&Aux,Data_S);
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~1);
				}
				if(WhatIsIt&2){
					Q_Shift(&Aux,Data_Q);
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~2);
				}
				if(WhatIsIt&4){
					H_Shift(&Aux,Data_H);
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~4);
				}
			}
		}
		if(WhatIsIt==0){
			printf("impossivel\n");
		}else if((WhatIsIt|1)==1){
			printf("pilha\n");
		}else if((WhatIsIt|2)==2){
			printf("fila\n");
		}else if((WhatIsIt|4)==4){
			printf("fila de prioridade\n");
		}else{
			printf("incerto\n");
		}
		S_Destroy(Data_S);
		Q_Destroy(Data_Q);
		H_Destroy(Data_H);
		scanf("%d",&N);
	}
	return EXIT_SUCCESS;
}