
/*
 *      == Lista Enumerada ==
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

#ifndef __List_H_
	#define __List_H_
	
	
	typedef int LIST_EL;
	typedef struct __List_t LIST;
	
	
	struct __List_t *L_New();
	
	int L_Size(struct __List_t *);
	
	char L_Add(LIST_EL,struct __List_t *);
	
	char L_AddAt(LIST_EL,int,struct __List_t *);
	
	char L_Remove(struct __List_t *);
	
	char L_RemoveAt(int,struct __List_t *);
	
	LIST_EL L_Get(struct __List_t *);
	
	LIST_EL L_GetAt(int,struct __List_t *);
	
	char L_Set(LIST_EL,struct __List_t *);
	
	char L_SetAt(LIST_EL,int,struct __List_t *);
	
	char L_Destroy(struct __List_t *L);
	
#endif

