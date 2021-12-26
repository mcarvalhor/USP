#include <stdlib.h>
#include <string.h>
#include <vectorutils.h>

void merge(void *vector, int size, int (*cmp)(void *,void *), int start, int middle, int end, int *CmpN, int *MovN) {
	int i, j, k;
	void *left, *right;
	int nleft, nright;

	nleft = middle - start + 1;
	nright = end - middle;

	left = (void *) malloc(size * nleft);
	right = (void *) malloc(size * nright);

	memcpy(left,vector+start*size,nleft*size);
	memcpy(right,vector+(middle+1)*size,nright*size);

	i = j = 0;

	for (k = start; k <= end; k++) {
		if (j>=nright || (i<nleft && ++(*CmpN) && cmp(left+i*size,right+j*size)>=0)) {
			(*MovN)+=3;
			gswap(vector+k*size,left+(i++)*size,size);
		} else if(j<nright) {
			(*MovN)+=3;
			gswap(vector+k*size,right+(j++)*size,size);
		}
	}

	free(left);
	free(right);
}

void mergesort_(void *vector, int size, int (*cmp)(void *,void *), int start, int end, int *CmpN, int *MovN) {
	
	if (start < end) {
		int middle = (start+end) / 2;
		mergesort_(vector, size, cmp, start, middle, CmpN, MovN);
		mergesort_(vector, size, cmp, middle+1, end, CmpN, MovN);
		merge(vector, size, cmp, start, middle, end, CmpN, MovN);
	}
}

void mergesort(void *vector, int size, int (*cmp)(void *,void *), int n, int *CmpN, int *MovN){
	int C = 0, M = 0;
	mergesort_(vector,size,cmp,0,n-1, &C, &M);
	*CmpN = C;
	*MovN = M;
}















