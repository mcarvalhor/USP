
/*
 *	~        SKIPLIST       ~
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

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

	typedef struct __dictionary_t SKIPLIST;

	struct __dictionary_t *New_Dictionary(unsigned int);

	int Insert_Word(char *, char *, struct __dictionary_t *);

	int Set_Word(char *, char *, struct __dictionary_t *);

	int Remove_Word(char *, struct __dictionary_t *);

	int Print_Word(char *, FILE *, struct __dictionary_t *);

	int Print_Words_Starting_With(char, FILE *, struct __dictionary_t *);

	void Destroy_Dictionary(struct __dictionary_t *);

#endif