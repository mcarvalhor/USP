
/*
 *	~      SUPER FILE       ~
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

#ifndef __SUPER_FILE_H_
#define __SUPER_FILE_H_

	char SF_PreviewChar(FILE *);

	int SF_ReadWhites(FILE *);

	int SF_ReadUntil(char,FILE *);

	char *SF_ReadString(FILE *);

	char *SF_ReadFormattedString(FILE *);

#endif
