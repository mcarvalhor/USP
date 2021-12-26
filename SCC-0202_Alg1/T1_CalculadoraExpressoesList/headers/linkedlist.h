
/*
 *      ~    Lista Enumerada    ~
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

#ifndef __LIST_H_
#define __LIST_H_
	
	
	#define LINKEDLIST_ELEM double
	typedef struct __List_t LIST;
	
	
	struct __List_t *L_New();
	
	int L_Size(struct __List_t *);
	
	char L_IsSubList(struct __List_t *);
	
	char L_IsSubListAt(int,struct __List_t *);
	
	char L_Add(LINKEDLIST_ELEM,struct __List_t *);
	
	char L_AddSubList(struct __List_t *,struct __List_t *);
	
	char L_AddAt(LINKEDLIST_ELEM,int,struct __List_t *);
	
	char L_AddSubListAt(struct __List_t *,int,struct __List_t *);
	
	char L_Remove(struct __List_t *);
	
	char L_RemoveAt(int,struct __List_t *);
	
	LINKEDLIST_ELEM L_Get(struct __List_t *);
	
	struct __List_t *L_GetSubList(struct __List_t *);
	
	LINKEDLIST_ELEM L_GetAt(int,struct __List_t *);
	
	struct __List_t *L_GetSubListAt(int,struct __List_t *);
	
	char L_Set(LINKEDLIST_ELEM,struct __List_t *);
	
	char L_SetSubList(struct __List_t *,struct __List_t *);
	
	char L_SetAt(LINKEDLIST_ELEM,int,struct __List_t *);
	
	char L_SetSubListAt(struct __List_t *,int,struct __List_t *);
	
	char L_Destroy(struct __List_t *L);
	
#endif
