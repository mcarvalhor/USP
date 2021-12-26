#include <vectorutils.h>


void bubblesort(void *vector, int size, int (*cmp)(void *,void *), int n, int *CmpN, int *MovN) {
	int i, j, C = 0, M = 0;
	for (j = 1; j < n; j++) {
		for (i = 0; i < n-j; i++) {
			C++;
			if (cmp(vector+i*size,vector+(i+1)*size)<0) {
				M+=3;
				gswap(vector+i*size, vector+(i+1)*size, size);
			}
		}
	}
	*CmpN = C;
	*MovN = M;
}
