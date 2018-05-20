
/*
 *	~      Rubro Negra      ~
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

#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_
#include <stdlib.h>

	typedef struct __redblacktree_t RBT;

	struct __redblacktree_t *RBT_New(size_t, int (*)(void *, void *), void (*)(void *));

	int RBT_Insert(void *, struct __redblacktree_t *);

	int RBT_Min(void *, struct __redblacktree_t *);

	int RBT_Max(void *, struct __redblacktree_t *);

	int RBT_Predecessor(void *, void *, struct __redblacktree_t *);

	int RBT_Successor(void *, void *, struct __redblacktree_t *);

	int RBT_PreOrder(void (*)(void *), struct __redblacktree_t *);

	int RBT_InOrder(void (*)(void *), struct __redblacktree_t *);

	int RBT_PostOrder(void (*)(void *), struct __redblacktree_t *);

	void RBT_Destroy(struct __redblacktree_t *);

#endif