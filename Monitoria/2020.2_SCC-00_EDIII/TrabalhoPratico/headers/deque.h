
/*
 *	~         Deque         ~
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

#ifndef DEQUE_H_
#define DEQUE_H_
#include <stdlib.h>



	typedef struct __deque_t DEQUE;



	struct __deque_t *DQ_New(size_t);

	long DQ_Size(struct __deque_t *);

	int DQ_PushFront(void *, struct __deque_t *);

	int DQ_PushBack(void *, struct __deque_t *);

	int DQ_Front(void *, struct __deque_t *);

	int DQ_Back(void *, struct __deque_t *);

	int DQ_ShiftFront(void *, struct __deque_t *);

	int DQ_ShiftBack(void *, struct __deque_t *);

	void DQ_Destroy(struct __deque_t *);



#endif