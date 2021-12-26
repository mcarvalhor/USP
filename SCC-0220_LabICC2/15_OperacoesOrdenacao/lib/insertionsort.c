#include <stdlib.h>
#include <string.h>
//#include <vectorutils.h>


void insertionsort(void *vector, int size, int (*cmp)(void *,void *), int n, int *CmpN, int *MovN) {
	int i, j, C = 0, M = 0;
	void *key = (void *) malloc(size);
	for (i = 1; i < n; i++) {
		j = i;
		M++;
		memcpy(key,vector+j*size,size);
		while (j > 0 && ++C && cmp(key,vector+(j-1)*size)>0) {
			M++;
			memcpy(vector+j*size,vector+(j-1)*size,size);
			j--;
		}
		M++;
		memcpy(vector+j*size,key,size);
	}
	*CmpN = C;
	*MovN = M;
	free(key);
}

