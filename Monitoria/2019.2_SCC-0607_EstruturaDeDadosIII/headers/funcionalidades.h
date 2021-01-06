
#ifndef FUNCIONALIDADES_H_
#define FUNCIONALIDADES_H_

	int f1_csvToBin(char *csvPath, char *binPath);
	int f2_listAll(char *binPath);
	int f3_listSearch(char *binPath);
	int f4_listRRN(int RRN, char *binPath);
	int f5_remove(char *binPath);
	int f6_insert(char *binPath);
	int f7_update(char *binPath);
	int f8_compactar(char *oldBinPath, char *newBinPath);

	int f9_gerarGrafo(char *binPath);
	int f10_caminhoMaisCurto(char *binPath);
	int f11_arvoreGeradoraMinima(char *binPath);

	int fx_barraR(char binary, char *filePath);
	int fx_invalidate(char *filePath);
	int fx_validate(char *filePath);

#endif