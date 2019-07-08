
#ifndef H_INDEX
#define H_INDEX
#include <stdio.h>
#include <register.h>

	typedef struct {
		long RRN;
		char key[120];
		long byteOffset;
	} IND_DATA;

	typedef struct {
		char status;
		int nItems;
	} IND_HEADER;

	typedef struct {
		IND_HEADER header;
		IND_DATA *data;
		char path[5000];
		long accessCount;
	} IND_T;

	#define IND_DATA_SIZE 128

	IND_T *I_init(char *path);
	int I_read(IND_T *index, char *path);
	int I_rewrite(IND_T *index);
	int I_write(IND_T *index, char *path);
	long I_count(IND_T *index);
	void I_resetCount(IND_T *index);	
	void I_destroy(IND_T *index);

	int I_push(REG_DATA *reg, IND_T *index);
	int I_pop(REG_DATA *reg, IND_T *index);
	int I_update(REG_DATA *reg, IND_T *index);
	IND_DATA *I_iterate(int *n, char *key, IND_T *index);


#endif
