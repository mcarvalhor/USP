
#ifndef H_FUNCIONALIDADES
#define H_FUNCIONALIDADES

	int f1_csvBuild(char *binPath, char *csvPath);
	int f2_list(char *binPath);
	int f3_search(char *binPath, char *field, char *value);
	int f4_removeRegs(char *binPath, int n, FILE *input);
	int f5_addRegs(char *binPath, int n, FILE *input);
	int f6_updateRegs(char *binPath, int n, FILE *input);
	int f7_sort(char *inPath, char *outPath);
	int f8_merge(char *in1Path, char *in2Path, char *outPath);
	int f9_match(char *in1Path, char *in2Path, char *outPath);
	int f10_index(char *binPath, char *indPath);
	int f11_searchKey(char *binPath, char *indPath, char *value);
	int f12_removeRegsKey(char *binPath, char *indPath, int n, FILE *input);
	int f13_addRegsKey(char *binPath, char *indPath, int n, FILE *input);
	int f14_searchKeyStats(char *binPath, char *indPath, char *value);
	int f15_indexB(char *binPath, char *indPath);
	int f16_searchKeyB(char *binPath, char *indPath, char *value);

	int fx_invalidate(char *binPath);
	int fx_listRemoved(char *binPath);

#endif
