#include <stdlib.h>
#include <time.h>
#include <vectorutils.h>

int partition(void *vector, int size, int (*cmp)(void *,void *), int left, int right, int *CmpN, int *MovN) {
	int j, i, pivot;

	pivot=(left+right)/2;
	gswap(vector+left*size, vector+pivot*size, size);
	(*MovN)+=3;

	for (i = left, j = left+1; j <= right; j++){
		(*CmpN)++;
		if (cmp(vector+j*size, vector+left*size) >= 0){
			(*MovN)+=3;
			gswap(vector+j*size, vector+(++i)*size, size);
		}
	}

	gswap(vector+i*size, vector+left*size, size);
	(*MovN)+=3;
	return i;
}

void quicksort_(void *vector, int size, int (*cmp)(void *,void *), int start, int end, int *CmpN, int *MovN) {
	int r;

	if (start < end) {
		r = partition(vector, size, cmp, start, end, CmpN, MovN);
		quicksort_(vector, size, cmp, start, r-1, CmpN, MovN);
		quicksort_(vector, size, cmp, r+1, end, CmpN, MovN);
	}
}

void quicksort(void *vector, int size, int (*cmp)(void *,void *), int n, int *CmpN, int *MovN) {
	int C = 0, M = 0;
	srand(time(NULL));
	quicksort_(vector,size,cmp,0,n-1, &C, &M);
	*CmpN = C;
	*MovN = M;
}


/*
Partition que usa pivot aleatório, mas com IF dentro do FOR.
int partition(int *vector, int left, int right) {
	int j, i, pivot;

	pivot=left+rand()%(right-left+1);

	for (i = left, j = left; j <= right; j++) {
		if (vector[j] < vector[pivot]) {
			if(pivot==i)	pivot=j;
			swap(vector, j, i);
			i++;
		}
	}

	swap(vector, i, pivot);
	return i;
}
*/

/*
Partition que usa pivot aleatório, mas sem IF dentro do FOR.
int partition(int *vector, int left, int right) {
	int j, i, pivot;

	pivot=left+rand()%(right-left+1);
	swap(vector,left,pivot);

	for (i = left, j = left+1; j <= right; j++) {
		if (vector[j] < vector[left]) {
			i++;
			swap(vector, j, i);
		}
	}

	swap(vector, i, left);
	return i;
}
*/

/*
Partition com pivot fixo (left).
int partition(int *vector, int left, int right) {
	int j, i;

	for (i = left, j = left+1; j <= right; j++) {
		if (vector[j] < vector[left]) {
			i++;
			swap(vector, j, i);
		}
	}

	swap(vector, i, left);
	return i;
}
*/

