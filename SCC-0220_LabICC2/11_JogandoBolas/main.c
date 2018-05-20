#include <stdlib.h>
#include <stdio.h>
#include <SortedHeap.h>



/*
 *	~    Jogando Bolas      ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



int main(int argc, char **argv){
	SORTEDHEAP *ItemsHeap;
	int Level, N, Aux=-1;
	scanf("%d %d",&Level,&N);
	ItemsHeap=New_SortedHeap(Level);
	for(;N>0;N--)	Aux=Traverse_ByStatus(ItemsHeap);
	Destroy_SortedHeap(ItemsHeap);
	printf("%d",Aux);
	return EXIT_SUCCESS;
}


