
#ifndef H_BUFFER
#define H_BUFFER
#include <stdio.h>

	#define PAGE_SIZE 32000
	#define BTREE_PAGE_SIZE 105

	unsigned long B_count(FILE *stream);
	unsigned long B_offset(FILE *stream);

#endif
