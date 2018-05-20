
/*
 *	~         STACK         ~
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

#ifndef __STACK_H_
#define __STACK_H_
	
	#define STACK_ELEM void*
	
	typedef struct __stack_t STACK;
	
	struct __stack_t *S_New();
	
	struct __stack_t *S_NewFrom(struct __stack_t *);
	
	char S_Cmp(struct __stack_t *,struct __stack_t *);
	
	char S_IsEmpty(struct __stack_t *);
	
	int S_Size(struct __stack_t *);
	
	char S_Push(STACK_ELEM,struct __stack_t *);
	
	STACK_ELEM S_Pop(struct __stack_t *);
	
	STACK_ELEM S_Get(struct __stack_t *);
	
	STACK_ELEM S_GetAt(int,struct __stack_t *);
	
	char S_Destroy(struct __stack_t *);
	
#endif
