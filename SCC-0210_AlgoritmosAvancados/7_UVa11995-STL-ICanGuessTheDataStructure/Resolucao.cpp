#include <stdlib.h>
#include <stdio.h>
#include <stack>
#include <queue>
using namespace std;

int main(int argc, char **argv){
	stack <int>Data_S;
	queue <int>Data_Q;
	priority_queue <int>Data_H;
	int i,N,Action,Element,Aux;
	char WhatIsIt;
	while(scanf("%d", &N) == 1){
		Data_S = stack <int>();
		Data_Q = queue <int>();
		Data_H = priority_queue <int>();
		WhatIsIt=7;
		for(i=0;i<N;i++){
			scanf("%d %d", &Action,&Element);
			if(Action==1){ // Adicionar
				if(WhatIsIt&1)	Data_S.push(Element);
				if(WhatIsIt&2)	Data_Q.push(Element);
				if(WhatIsIt&4)	Data_H.push(Element);
			}else{ // Remover
				if(WhatIsIt&1){
					Aux = Data_S.top();
					Data_S.pop();
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~1);
				}
				if(WhatIsIt&2){
					Aux = Data_Q.front();
					Data_Q.pop();
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~2);
				}
				if(WhatIsIt&4){
					Aux = Data_H.top();
					Data_H.pop();
					if(Aux!=Element)	WhatIsIt=WhatIsIt&(~4);
				}
			}
		}
		if(WhatIsIt==0){
			printf("impossible\n");
		}else if((WhatIsIt|1)==1){
			printf("stack\n");
		}else if((WhatIsIt|2)==2){
			printf("queue\n");
		}else if((WhatIsIt|4)==4){
			printf("priority queue\n");
		}else{
			printf("not sure\n");
		}
	}
	return EXIT_SUCCESS;
}
