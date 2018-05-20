
/*
 *	~  Calc. de Expressões  ~
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

#ifndef __CALC_H_
#define __CALC_H_
	#include "linkedlist.h"

	LIST *NewExpression(FILE *);

	char CalcExpression(double *,LIST *);

	char DestroyExpression(LIST *);

#endif
