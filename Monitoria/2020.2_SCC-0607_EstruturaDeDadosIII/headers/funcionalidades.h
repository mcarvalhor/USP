
#ifndef FUNCIONALIDADES_H_
#define FUNCIONALIDADES_H_

	void f1_readCsv(char *csvPath, char *dataPath, char *indexPath);
	void f2_listAll(char *dataPath);
	void f3_search(char *dataPath, char *indexPath);
	void f4_insert(char *dataPath, char *indexPath);
	void f5_update(char *dataPath, char *indexPath);
	void f6_readCsv(char *csvPath, char *dataPath);
	void f7_sort(char *dataPath, char *sortedDataPath);
	void f8_listMatch(char *f1Path, char *f1IndPath, char *f2Path, char *key);
	void f9_graph(char *f1Path, char *f1IndPath, char *f2Path);
	void f10_graphInverted(char *f1Path, char *f1IndPath, char *f2Path);
	void f11_bfs(char *f1Path, char *f1IndPath, char *f2Path, char *key);
	void f12_dfsCycle(char *f1Path, char *f1IndPath, char *f2Path, char *key);

	void fx_invalidadeStatus(char *filePath);
	void fx_compressFile2(char *file2Path);
	
#endif