
/*
 *	~      Tabela Hash      ~
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

#ifndef HASHTREE_H_
#define HASHTREE_H_
#include <stdio.h>


	typedef struct __hashtree_t HASHTREE;


	struct __hashtree_t *HTree_New(unsigned long);

	int HTree_Insert(int, struct __hashtree_t *);

	int HTree_Remove(int, struct __hashtree_t *);

	int HTree_Count(int, struct __hashtree_t *);

	int HTree_Traverse(FILE *, struct __hashtree_t *);

	void HTree_Destroy(struct __hashtree_t *T);


#endif