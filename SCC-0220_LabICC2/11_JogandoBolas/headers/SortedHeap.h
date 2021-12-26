
/*
 *	~      SortedHeap       ~
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

#ifndef SORTEDHEAP_H_
#define SORTEDHEAP_H_

	typedef struct __sortedheap_t SORTEDHEAP;

	struct __sortedheap_t *New_SortedHeap(int);

	int Traverse_ByStatus(struct __sortedheap_t *);

	void Destroy_SortedHeap(struct __sortedheap_t *);

#endif