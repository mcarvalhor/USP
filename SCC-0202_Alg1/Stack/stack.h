
/*
 *	~         Pilha         ~
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

#ifndef STACK_H_
#define STACK_H_
#include <stdlib.h>



	typedef struct __stack_t STACK;



	struct __stack_t *S_New(size_t);

	long S_Size(struct __stack_t *);

	int S_Push(void *, struct __stack_t *);

	int S_Top(void *, struct __stack_t *);

	int S_Bottom(void *, struct __stack_t *);

	int S_Pop(void *, struct __stack_t *);

	void S_Destroy(struct __stack_t *);



#endif
