
/*
 *	~         SOCCER        ~
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

#ifndef __SOCCER_H_
#define __SOCCER_H_

	typedef struct __soccer_tour_t SOCCER_TOUR;

	struct __soccer_tour_t *S_NewFrom(FILE *);

	char S_Calc(struct __soccer_tour_t *);

	char S_Sort(struct __soccer_tour_t *);

	char S_Print(struct __soccer_tour_t *,FILE *);

	char S_Destroy(struct __soccer_tour_t *);

#endif
