#include <vectorutils.h>

#define ROOT 0
#define right(pos) pos*2+2
#define left(pos) pos*2+1

void max_heapify(void *vector, int size, int (*cmp)(void *,void *), int n, int position, int *CmpN, int *MovN) {
	int largestId, leftId, rightId;

	largestId = position;
	leftId = left(position);
	rightId = right(position);

	if (leftId < n && ++(*CmpN) && cmp(vector+leftId*size,vector+largestId*size)<0)
		largestId = leftId;
	if (rightId < n && ++(*CmpN) && cmp(vector+rightId*size,vector+largestId*size)<0)
		largestId = rightId;

	if (largestId != position) {
		(*MovN)+=3;
		gswap(vector+largestId*size, vector+position*size, size);
		max_heapify(vector, size, cmp, n, largestId, CmpN, MovN);
	}
}

void heapsort(void *vector, int size, int (*cmp)(void *,void *), int n, int *CmpN, int *MovN) {
	int i, C = 0, M = 0;

	for (i = n/2-1; i >= 0; i--)	max_heapify(vector, size, cmp, n, i, &C, &M);

	for (i = n-1; i > ROOT; i--) {
		M+=3;
		gswap(vector+ROOT*size, vector+i*size, size);
		max_heapify(vector, size, cmp, i, ROOT, &C, &M);
	}

	*CmpN = C;
	*MovN = M;
}
