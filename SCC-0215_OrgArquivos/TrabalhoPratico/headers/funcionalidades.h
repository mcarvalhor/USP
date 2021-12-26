
/*
 *	~       Trabalho Prático: Parte 2       ~
 *
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



#ifndef FUNCIONALIDADES_H_
#define FUNCIONALIDADES_H_


	void transferirCSV(char *nomeArquivoCSV, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void recuperarTudo(char *nomeArquivoDAT);

	void recuperarPorCampo(char *nomeCampo, char* valorCampo, char *nomeArquivoDAT);

	void recuperarPorRRN(int RRN, char *nomeArquivoDAT);

	void removerRegistro(int RRN, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void inserirRegistro(char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void atualizarRegistro(int RRN, char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void desfragmentarBinario(char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void imprimirPilha(char *nomeArquivoDAT);

	void recuperarPorChave(int Chave, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void removerRegistroChave(int Chave, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);

	void atualizarRegistroChave(int Chave, char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador);


#endif
