
#ifndef H_QUEUE_
#define H_QUEUE_
#include <stdio.h>

	typedef struct _QUEUE_T QUEUE_T;


	QUEUE_T *Q_new(size_t);
	int Q_push(void *, QUEUE_T *);
	int Q_shift(void *, QUEUE_T *);
	unsigned long Q_size(QUEUE_T *);
	int Q_clear(QUEUE_T *);
	void Q_destroy(QUEUE_T *);

#endif