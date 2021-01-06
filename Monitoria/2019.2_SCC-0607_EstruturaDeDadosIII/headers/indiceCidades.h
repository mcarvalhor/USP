
#ifndef INDICECIDADES_H_
#define INDICECIDADES_H_

	typedef struct __INDEX_T INDEX_T;

	INDEX_T *I_new();
	INDEX_T *I_newFrom(char *filename);

	int I_insert(char *key, INDEX_T *ind);
	int I_insertCount(char *key, unsigned long count, INDEX_T *ind);
	int I_remove(char *key, INDEX_T *ind);
	int I_removeCount(char *key, unsigned long count, INDEX_T *ind);
	long I_keyCount(INDEX_T *ind);

	int I_fillRestOfKey(char *key, INDEX_T *ind);
	long I_getIndexFor(char *key, INDEX_T *ind);
	char *I_getKeyFor(long index, INDEX_T *ind);

	int I_saveTo(char *filename, INDEX_T *ind);
	int I_loadFrom(char *filename, INDEX_T *ind);

	void I_destroy(INDEX_T *ind);

#endif