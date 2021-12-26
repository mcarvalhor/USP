
#ifndef H_VECTOR_BUILDER_
#define H_VECTOR_BUILDER_
#include <stdio.h>

	typedef struct __VECTOR_T VECTOR_T;


	VECTOR_T *V_new(size_t);
	int V_pushback(void *, VECTOR_T *);
	unsigned long V_size(VECTOR_T *);
	unsigned long V_copy(void *, VECTOR_T *);
	unsigned long V_build(void *, VECTOR_T *);
	int V_clear(VECTOR_T *);
	void V_destroy(VECTOR_T *v);

#endif