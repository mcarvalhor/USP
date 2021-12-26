
/*
 *	~         Filas         ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/

#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdlib.h>



	typedef struct __queue_t QUEUE;
	typedef struct __priority_queue_t PRIORITY_QUEUE;



	struct __queue_t *Q_New(size_t);

	long Q_Size(struct __queue_t *);

	int Q_Push(void *, struct __queue_t *);

	int Q_Front(void *, struct __queue_t *);

	int Q_Back(void *, struct __queue_t *);

	int Q_Shift(void *, struct __queue_t *);

	void Q_Destroy(struct __queue_t *);



	struct __priority_queue_t *PQ_New(size_t, int (*)(void *, void *));

	long PQ_Size(struct __priority_queue_t *);

	int PQ_Push(void *, struct __priority_queue_t *);

	int PQ_Front(void *, struct __priority_queue_t *);

	int PQ_Back(void *, struct __priority_queue_t *);

	int PQ_Shift(void *, struct __priority_queue_t *);

	void PQ_Destroy(struct __priority_queue_t *);



#endif
