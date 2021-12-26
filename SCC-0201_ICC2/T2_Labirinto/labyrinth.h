
/*
 *	~        LABYRINTH      ~
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

#ifndef __Labyrinth_H_
#define __Labyrinth_H_
	
	typedef struct __lab_t LABYRINTH;
	
	struct __lab_t *ReadLab(FILE *);
	
	char FindSolutionsWithoutTreasure(struct __lab_t *);
	
	char FindSolutionsWithTreasure(struct __lab_t *);
	
	char SortSolutions(struct __lab_t *);
	
	char PrintSolutions(struct __lab_t *,FILE *);
	
	char DestroyLab(struct __lab_t *);
	
#endif
