
#ifndef H_BINDEX
#define H_BINDEX
#include <stdio.h>
#include <register.h>

	#define BTR_M 7
	#define BTR_TRASH -1
	#define BTR_IS_LEAVE '1'
	#define BTR_IS_NOT_LEAVE '0'

	typedef struct {
		char status;
		int root;
	} BTR_HEADER;

	typedef struct {
		int nodeRRN;
		char leave;
		int nItems;
		int key[BTR_M - 1];
		long byteOffset[BTR_M - 1];
		int pointers[BTR_M];
	} BTR_NODE;

	typedef struct {
		BTR_HEADER header;
		long levelCount;
		FILE *stream;
	} BTR_T;

	#define IND_DATA_SIZE 128

	BTR_T *BT_init(char *path);
	BTR_T *BT_new(char *path);
	long BT_count(BTR_T *index);
	void BT_resetCount(BTR_T *index);	
	int BT_flush(BTR_T *index);
	int BT_close(BTR_T *index);

	int BT_push(REG_DATA *reg, BTR_T *index);
	/*int BT_pop(REG_DATA *reg, BTR_T *index);
	int BT_update(REG_DATA *reg, BTR_T *index);*/
	long BT_search(int key, BTR_T *index);


#endif
