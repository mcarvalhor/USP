
#ifndef INDEX_H_
#define INDEX_H_
#include "register.h"

	typedef struct {
		int key;
		int RRN;
	} IND_DATA;

	typedef struct {
		char status;
		int nItems;
		IND_DATA *data;
		char path[5000];
	} IND_T;

	#define IND_DATA_SIZE 128

	IND_T *I_init(char *path);
	int I_read(IND_T *index, char *path);
	int I_rewrite(IND_T *index);
	int I_write(IND_T *index, char *path);
	void I_destroy(IND_T *index);

	int I_push(REG_DATA *reg, IND_T *index);
	int I_pop(REG_DATA *reg, IND_T *index);
	int I_update(REG_DATA *reg, IND_T *index);
	IND_DATA *I_find(int key, IND_T *index);


#endif