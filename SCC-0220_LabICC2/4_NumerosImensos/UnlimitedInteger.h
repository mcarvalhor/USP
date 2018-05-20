
/*
 *      == Inteiros Ilimitados ==
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

#ifndef __UnlimitedInteger_H_
	#define __UnlimitedInteger_H_
	
	
	typedef struct __UI_t UI;
	
	
	struct __UI_t *UI_New();
	
	struct __UI_t *UI_NewFromInt(long int);
	
	struct __UI_t *UI_Read(FILE *);
	
	char UI_Write(FILE *, struct __UI_t *);
	
	char UI_Cmp(struct __UI_t *, struct __UI_t *);
	
	struct __UI_t *UI_Abs(struct __UI_t *);
	
	struct __UI_t *UI_Sum(struct __UI_t *,struct __UI_t *);
	
	struct __UI_t *UI_Subtraction(struct __UI_t *,struct __UI_t *);
	
	struct __UI_t *UI_Product(struct __UI_t *,struct __UI_t *);
	
	struct __UI_t *UI_Quotient(struct __UI_t *,struct __UI_t *);
	
	char UI_Destroy(struct __UI_t *);
	
#endif
