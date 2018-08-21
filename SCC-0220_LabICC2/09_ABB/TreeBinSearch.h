
/*
 *      ==         ABB         ==
 *
 *      Matheus Carvalho Raimundo
 *      mcarvalhor@usp.br
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/

#ifndef __BIN_SEARCH_TREE_H_
#define __BIN_SEARCH_TREE_H_

	typedef struct __bin_search_tree BIN_SEARCH_TREE;

	struct __bin_search_tree *Create_Tree();

	char Find_In(int,struct __bin_search_tree *);

	char Insert_Into(int,struct __bin_search_tree *);

	char Remove_First_From(int,struct __bin_search_tree *);

	char Print_InOrder(FILE *,struct __bin_search_tree *);

	char Print_PreOrder(FILE *,struct __bin_search_tree *);

	char Print_PostOrder(FILE *,struct __bin_search_tree *);

	char Print_LevelOrder(FILE *,struct __bin_search_tree *);

	char Destroy_Tree(struct __bin_search_tree *);

#endif