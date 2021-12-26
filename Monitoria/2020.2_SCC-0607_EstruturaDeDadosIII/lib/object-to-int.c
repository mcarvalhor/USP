#include <stdlib.h>
#include <string.h>



typedef struct __objtoint_node {
	void *object;
	struct __objtoint_node *next;
} NODE;

typedef struct __objtoint {
	int (*compar)(const void *, const void *);
	void *vector;
	NODE *root;
	long count;
	size_t size;
} OBJTOINT;



OBJTOINT *OBJ_Init(size_t Size, int (*Compar)(const void *, const void *)) {
	OBJTOINT *aux;

	if(Size < 1) {
		return NULL;
	}

	aux = (OBJTOINT *) malloc(sizeof(OBJTOINT));
	if(aux == NULL) {
		return NULL;
	}

	aux->compar = Compar;
	aux->vector = NULL;
	aux->root = NULL;
	aux->size = Size;
	aux->count = 0;
	return aux;
}

int OBJ_Push(void *Obj, OBJTOINT *O) {
	NODE *aux;
	if(Obj == NULL || O == NULL || O->vector != NULL) {
		return 0;
	}

	aux = (NODE *) malloc(sizeof(NODE));
	if(aux == NULL) {
		return 0;
	}

	aux->object = (void *) malloc(O->size);
	memcpy(aux->object, Obj, O->size);

	aux->next = O->root;
	O->root = aux;
	O->count++;
	return 1;
}

int OBJ_BuildVector(OBJTOINT *O) {
	NODE *aux, *tmp;
	long i;
	if(O == NULL || O->count < 1) {
		return 0;
	}

	if(O->vector != NULL) {
		return 1;
	}

	O->vector = (void *) malloc(O->size * O->count);
	if(O->vector == NULL) {
		return 0;
	}

	for(i = 0, aux = O->root; aux != NULL; aux = aux->next) {
		memcpy(O->vector + O->size * (i++), aux->object, O->size);
	}

	aux = O->root;
	while(aux != NULL) {
		tmp = aux;
		aux = aux->next;
		free(tmp->object);
		free(tmp);
	}
	O->root = NULL;

	qsort(O->vector, O->count, O->size, O->compar);
	return 1;
}

long OBJ_Count(OBJTOINT *O) {
	if(O == NULL) {
		return -1;
	}
	return O->count;
}

long OBJ_IndexFor(void *Obj, OBJTOINT *O) {
	long start, end, middle, cmp;
	if(O == NULL || Obj == NULL || O->vector == NULL) {
		return -2;
	}

	for(start = 0, end = O->count - 1; start <= end;) {
		middle = (start + end) / 2;
		cmp = O->compar(O->vector + middle * O->size, Obj);
		if(cmp < 0) {
			start = middle + 1;
		} else if(cmp > 0) {
			end = middle - 1;
		} else {
			return middle;
		}
	}
	return -1;
}

int OBJ_ObjectFor(long index, void *ret, OBJTOINT *O) {
	if(O == NULL || O->vector == NULL || index < 0 || index >= O->count) {
		return 0;
	}

	if(ret != NULL) {
		memcpy(ret, O->vector + index * O->size, O->size);
	}
	return 1;
}

void OBJ_Destroy(OBJTOINT *O) {
	NODE *aux, *tmp;
	if(O == NULL) {
		return;
	}
	aux = O->root;
	while(aux != NULL) {
		tmp = aux;
		aux = aux->next;
		free(tmp->object);
		free(tmp);
	}
	if(O->vector != NULL) {
		free(O->vector);
	}
	free(O);
}


