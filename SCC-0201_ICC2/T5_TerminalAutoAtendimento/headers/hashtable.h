
/*
 *	~       HashTable       ~
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

#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include <stdlib.h>


	typedef struct __hashtable_t HASHTABLE;


	struct __hashtable_t *New_HashTable(size_t, void (*)(void *), unsigned long );

	int Insert_Into_HashTable(long, void *, struct __hashtable_t *);

	unsigned long Search_From_HashTable(long, void *, struct __hashtable_t *);

	int Traverse_HashTable(void (*)(void *), struct __hashtable_t *);

	void Destroy_HashTable(struct __hashtable_t *);

#endif