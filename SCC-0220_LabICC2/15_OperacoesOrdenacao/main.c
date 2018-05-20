#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <insertionsort.h>
#include <bubblesort.h>
#include <mergesort.h>
#include <heapsort.h>
#include <quicksort.h>



/*
 *	~    Op. Ordenacao      ~
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


typedef enum { insertion_sort = 0, bubble_sort, merge_sort, heap_sort, quick_sort } SORTING_NAME;

typedef struct {
	SORTING_NAME Type;
	char Name[101];
	int C;
	int M;
} SORTING_METHOD;

int compare_integer(void *, void *);
int compare_C(void *, void *);
int compare_M(void *, void *);

int main(int argc, char **argv){
	SORTING_METHOD *Algorithms;
	int i, O, N, AuxInt;
	int *Vector, *AuxVector;
	
	scanf("%d %d", &O, &N);
	Algorithms = (SORTING_METHOD *) malloc(sizeof(SORTING_METHOD) * O);
	Vector = (int *) malloc(sizeof(int) * N);
	AuxVector = (int *) malloc(sizeof(int) * N);

	for(i=0; i<O; i++){
		scanf("%100s", Algorithms[i].Name);
		if(!strcmp(Algorithms[i].Name, "INSERTIONSORT"))	Algorithms[i].Type = insertion_sort;
		else if(!strcmp(Algorithms[i].Name, "BUBBLESORT"))	Algorithms[i].Type = bubble_sort;
		else if(!strcmp(Algorithms[i].Name, "MERGESORT"))	Algorithms[i].Type = merge_sort;
		else if(!strcmp(Algorithms[i].Name, "HEAPSORT"))	Algorithms[i].Type = heap_sort;
		else	Algorithms[i].Type = quick_sort;
	}
	for(i=0;i<N;i++){
		scanf("%d", &AuxInt);
		Vector[i] = AuxInt;
	}

	for(i=0; i<O; i++){
		memcpy(AuxVector, Vector, sizeof(int) * N);
		switch(Algorithms[i].Type){
			case insertion_sort:
				insertionsort(AuxVector, sizeof(int), compare_integer, N, &Algorithms[i].C, &Algorithms[i].M);
			break;
			case bubble_sort:
				bubblesort(AuxVector, sizeof(int), compare_integer, N, &Algorithms[i].C, &Algorithms[i].M);
			break;
			case merge_sort:
				mergesort(AuxVector, sizeof(int), compare_integer, N, &Algorithms[i].C, &Algorithms[i].M);
			break;
			case heap_sort:
				heapsort(AuxVector, sizeof(int), compare_integer, N, &Algorithms[i].C, &Algorithms[i].M);
			break;
			default:
				quicksort(AuxVector, sizeof(int), compare_integer, N, &Algorithms[i].C, &Algorithms[i].M);
			break;
		}
	}

	mergesort(Algorithms, sizeof(SORTING_METHOD), compare_C, O, &AuxInt, &AuxInt);
	printf("Menor C: %s\nMaior C: %s\n", Algorithms[0].Name, Algorithms[O-1].Name);

	mergesort(Algorithms, sizeof(SORTING_METHOD), compare_M, O, &AuxInt, &AuxInt);
	printf("Menor M: %s\nMaior M: %s\n", Algorithms[0].Name, Algorithms[O-1].Name);

	free(Vector);
	free(AuxVector);
	free(Algorithms);

	return EXIT_SUCCESS;
}


int compare_integer(void *PA, void *PB){
	int *A=PA, *B=PB;
	return (*B)-(*A);
}

int compare_C(void *PA, void *PB){
	SORTING_METHOD *A=PA, *B=PB;
	return B->C - A->C;
}

int compare_M(void *PA, void *PB){
	SORTING_METHOD *A=PA, *B=PB;
	return B->M - A->M;
}
