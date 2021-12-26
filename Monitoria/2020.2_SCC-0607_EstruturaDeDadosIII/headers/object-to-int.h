
#ifndef __OBJTOINT_
#define __OBJTOINT_
#include <stdlib.h>

	typedef struct __objtoint OBJTOINT;

	OBJTOINT *OBJ_Init(size_t Size, int (*Compar)(const void *, const void *));
	int OBJ_Push(void *Obj, OBJTOINT *O);
	int OBJ_BuildVector(OBJTOINT *O);
	long OBJ_Count(OBJTOINT *O);
	long OBJ_IndexFor(void *Obj, OBJTOINT *O);
	int OBJ_ObjectFor(long index, void *ret, OBJTOINT *O);
	void OBJ_Destroy(OBJTOINT *O);

#endif
