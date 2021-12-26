
/*
 *	~         Cartas        ~
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

#ifndef __CARDS_H_
#define __CARDS_H_
#include <list.h>
	
	LIST *ReadCardsFrom(FILE *);
	
	char ProcessCards(FILE *,LIST *);
	
	char DestroyCards(LIST *);
	
#endif
