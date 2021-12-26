#ifndef QUEUE_H_
#define QUEUE_H_
//#include <stdlib.h>

	typedef struct __queue_t QUEUE;

	struct __queue_t *Q_New(size_t);

	int Q_Size(struct __queue_t *);

	char Q_Add(void *, struct __queue_t *);

	int Q_Get(void *, struct __queue_t *);

	int Q_Shift(void *, struct __queue_t *);

	void Q_Destroy(struct __queue_t *);

#endif