
/*
 *	~    Sliding Puzzle     ~
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

#ifndef SLIDING_PUZZLE_H_
#define SLIDING_PUZZLE_H_
#include <stdio.h>


	typedef struct __slidingpuzzle_t PUZZLE;


	struct __slidingpuzzle_t *New_Puzzle(FILE *, unsigned long);

	int Move_Puzze_Up(struct __slidingpuzzle_t *);

	int Move_Puzze_Left(struct __slidingpuzzle_t *);

	int Move_Puzze_Down(struct __slidingpuzzle_t *);

	int Move_Puzze_Right(struct __slidingpuzzle_t *);

	int Print_Puzzle(FILE *, struct __slidingpuzzle_t *);

	void Destroy_Puzzle(struct __slidingpuzzle_t *);


#endif