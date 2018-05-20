
/*
 *      ==     IMAGENS P&B     ==
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

#ifndef __IMAGE_H_
#define __IMAGE_H_

	typedef struct __img_t IMG;

	struct __img_t *New_Img();

	char Read_Img(FILE *,struct __img_t *);

	struct __img_t *Join_Img(struct __img_t *,struct __img_t *);

	int Count_Img_Filled_Pixels(int,struct __img_t *);

	char Destroy_Img(struct __img_t *);

#endif