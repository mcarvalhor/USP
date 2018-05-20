#ifndef HEAP_H_
#define HEAP_H_
//#include <stdlib.h>

	typedef struct __heap_t HEAP;

	struct __heap_t *H_New(size_t, int (*)(void *, void *));

	int H_Add(void *, struct __heap_t *);

	int H_Get(void *, struct __heap_t *);

	int H_Shift(void *, struct __heap_t *);

	void H_Destroy(struct __heap_t *);

#endif