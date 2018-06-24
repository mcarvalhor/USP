#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cabecalho.h>
#include <arvore-b.h>



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



/* == Definições de pré-compilação == */


#define VERDADEIRO 1
#define FALSO 0

#define DATA_NULA "\0\0\0\0\0\0\0\0\0\0"
#define UF_NULO "\0\0"

#define REGISTRO_NULO '\0'

#define TAMANHO_REGISTRO 87
#define MASCARA_REMOCAO -1
#define FIM_TOPO_PILHA -1



/* == Declaração da estrutura do registro == */

typedef struct {
	/* Campos de tamanho fixo */
	int codINEP; /* No caso de registro removido, este campo é igual a MASCARA_REMOCAO */
	char dataAtiv[10];
	char uf[2];
	/* Indicadores de tamanho + campos de tamanho variável */
	int indTamEscola; /* No caso de registro removido, este campo assume o topo da pilha */
	char *nomeEscola;
	int indTamMunicipio;
	char *municipio;
	int indTamPrestadora;
	char *prestadora;
} REGISTRO;


/* Funções úteis, usadas por todas ou grande parte das funcionalidades do trabalho
   Estas se encontram ao fim deste arquivo *.c, logo após a implementação das funcionalidades do trabalho */

int lerRegistro(REGISTRO *reg, FILE *arquivoDAT);
int lerRegistroCSV(FILE *arquivoCSV, REGISTRO *reg);
void escreverRegistro(REGISTRO *reg, FILE *arquivoDAT);

void imprimirRegistro(REGISTRO *reg);
void destruirRegistro(REGISTRO *reg);



/* == Implementação das funcionalidades do trabalho == */


/* FUNCIONALIDADE 1 */
void transferirCSV(char *nomeArquivoCSV, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função transfere todos os registros contidos no arquivo *.CSV 'nomeArquivoCSV'
	*   para um novo arquivo binário 'nomeArquivoDAT'.
	*
	* Se o arquivo binário 'nomeArquivoDAT' já existe, ele é sobrescrito do zero.
	*/
	FILE *arquivoEntrada, *arquivoBinario, *arquivoIndices;
	REGISTRO reg;
	int aux, i;

	/* Abre o arquivo de entrada *.CSV para organizar os registros em um arquivo binário */
	arquivoEntrada = fopen(nomeArquivoCSV, "r");
	if (arquivoEntrada == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return;
	}

	/* Cria o arquivo binário */
	arquivoBinario = fopen(nomeArquivoDAT, "w+b");
	if (arquivoBinario == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(arquivoEntrada);
		return;
	}

	/* Cria o arquivo de índices árvore-B */
	arquivoIndices = fopen(nomeArquivoIndices, "w+b");
	if (arquivoIndices == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		fclose(arquivoEntrada);
		fclose(arquivoBinario);
		return;
	}

	cab.status = FALSO;
	cab.topoPilha = FIM_TOPO_PILHA;
	escreverCabecalho(arquivoBinario);

	cabIndice.status = FALSO;
	cabIndice.altura = 0;
	cabIndice.noRaiz = cabIndice.ultimoRRN = -1;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	i = 0;
	/* Enquanto houverem registros no *.CSV... */
	while ((aux = lerRegistroCSV(arquivoEntrada, &reg)) != 0) {
		if (aux == -1) /* Código INEP nulo? Não pode! Pular este registro */
			continue;
		/* Registro ok. Copiar ele para o arquivo binário e adicionar chave na árvore-B. */
		escreverRegistro(&reg, arquivoBinario);
		inserirChave(reg.codINEP, i++, arquivoIndices);
		destruirRegistro(&reg);
	}

	cab.status = VERDADEIRO;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Arquivo carregado.\n");
	fclose(arquivoEntrada);
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 2 */
void recuperarTudo(char *nomeArquivoDAT) {
	/*
	* Esta função lê e imprime em stdout todos os registros contidos no arquivo 'nomeArquivoDat'.
	*/
	FILE *arquivoBinario;
	int aux, contador;
	REGISTRO reg;

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if (cab.status != VERDADEIRO) {
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	contador = 0;
	/* Enquanto houverem registros para serem lidos no arquivo binário... */
	while((aux = lerRegistro(&reg, arquivoBinario)) != 0) {
		if(aux == -1) /* Registro removido, ignorar */
			continue;
		/* Imprime o registro */
		imprimirRegistro(&reg);
		destruirRegistro(&reg);
		contador++;
	}

	if(contador < 1){
		/* Caso em que nenhum registro foi encontrado */
		printf("Registro inexistente.\n");
	}

	fclose(arquivoBinario);
}


/* FUNCIONALIDADE 3 */
void recuperarPorCampo(char *nomeCampo, char* valorCampo, char *nomeArquivoDAT){
	/*
	* Esta função busca no arquivo binário 'nomeArquivoDAT' por registros em que o
	*   campo 'nomeCampo' tenha valor igual a 'valorCampo'.
	* Ela imprime em stdout os registros que satisfazem essa condição.
	*/
	int aux, contador, campo;
	FILE *arquivoBinario;
	REGISTRO reg;

	if(strcmp(nomeCampo, "codINEP") == 0) {
		campo = 1;
	} else if(strcmp(nomeCampo, "dataAtiv") == 0) {
		campo = 2;
	} else if(strcmp(nomeCampo, "uf") == 0) {
		campo = 3;
	} else if(strcmp(nomeCampo, "nomeEscola") == 0) {
		campo = 4;
	} else if(strcmp(nomeCampo, "municipio") == 0) {
		campo = 5;
	} else if(strcmp(nomeCampo, "prestadora") == 0) {
		campo = 6;
	} else {
		/* Nome do campo inválido. */
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if (cab.status != VERDADEIRO) {
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	contador = 0;
	/* Enquanto houverem registros para serem lidos no arquivo binário... */
	while((aux = lerRegistro(&reg, arquivoBinario)) != 0) {
		if(aux == -1) /* Registro removido. */
			continue;
		switch(campo){
			case 1:
				if(reg.codINEP == atoi(valorCampo)){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
			case 2:
				if(strncmp(reg.dataAtiv, valorCampo, 10) == 0){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
			case 3:
				if(strncmp(reg.uf, valorCampo, 2) == 0){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
			case 4:
				if(strcmp(reg.nomeEscola, valorCampo) == 0){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
			case 5:
				if(strcmp(reg.municipio, valorCampo) == 0){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
			case 6:
				if(strcmp(reg.prestadora, valorCampo) == 0){
					imprimirRegistro(&reg);
					contador++;
				}
			break;
		}
		destruirRegistro(&reg);
	}

	if(contador < 1){
		/* Caso em que nenhum registro que satisfaz a condição foi impresso */
		printf("Registro inexistente.\n");
	}

	fclose(arquivoBinario);
}


/* FUNCIONALIDADE 4 */
void recuperarPorRRN(int RRN, char *nomeArquivoDAT) {
	/*
	* Esta função imprime em stdout apenas o registro de RRN 'RRN' do arquivo binário 'nomeArquivoDAT'.
	*/
	FILE *arquivoBinario;
	REGISTRO reg;

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	if (fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		fclose(arquivoBinario);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		fclose(arquivoBinario);
		return;
	}

	/* Impressão do registro */
	imprimirRegistro(&reg);

	destruirRegistro(&reg);
	fclose(arquivoBinario);
}


/* FUNCIONALIDADE 5 */
void removerRegistro(int RRN, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função remove do arquivo binário 'nomeArquivoDAT' o registro de RRN 'RRN'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;

	arquivoBinario = fopen(nomeArquivoDAT, "r+b");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "r+b");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cab.status = FALSO;
	escreverCabecalho(arquivoBinario);

	if (fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cabIndice.status = FALSO;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	/* Remover chave do registro da árvore-B */
	removerChave(reg.codINEP, arquivoIndices);
	destruirRegistro(&reg);

	/* Voltar para poder reescrever como removido */
	fseek(arquivoBinario, -1 * TAMANHO_REGISTRO, SEEK_CUR);

	/* Atribuir a MASCARA_REMOCAO e o topo da pilha */
	reg.codINEP = MASCARA_REMOCAO;
	reg.indTamEscola = cab.topoPilha;

	/* Reescrever como removido */
	escreverRegistro(&reg, arquivoBinario);

	/* Atualiza o topo da pilha com o RRN do registro que foi removido */
	cab.status = VERDADEIRO;
	cab.topoPilha = RRN;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Registro removido com sucesso.\n");
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 6 */
void inserirRegistro(char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função insere no arquivo binário 'nomeArquivoDAT' um novo registro.
	* Os campos para este novo registro são passados de forma ordenada (0-5) em 'campos'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;
	int i;

	/* Validação dos campos */
	if(strlen(campos[1]) != 10 && (campos[1][0] != '0' || campos[1][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[2]) != 2 && (campos[2][0] != '0' || campos[2][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[3]) + strlen(campos[4]) + strlen(campos[5]) > TAMANHO_REGISTRO - sizeof(int) * 4 - sizeof(char) * 12){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	arquivoBinario = fopen(nomeArquivoDAT, "r+b");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "r+b");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cab.status = FALSO;
	escreverCabecalho(arquivoBinario);

	cabIndice.status = FALSO;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	if (cab.topoPilha == FIM_TOPO_PILHA) {
		/* Não há registros removidos na pilha, inserir no final do arquivo */
		fseek(arquivoBinario, 0, SEEK_END);
		i = (ftell(arquivoBinario) - TAMANHO_CABECALHO) / TAMANHO_REGISTRO;
	} else {
		/* Existe pelo menos um registro removido na pilha, inserir em sua posição */
		i = cab.topoPilha;
		fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * cab.topoPilha, SEEK_SET);
		lerRegistro(&reg, arquivoBinario);
		cab.topoPilha = reg.indTamEscola;
		fseek(arquivoBinario, -1 * TAMANHO_REGISTRO, SEEK_CUR); /* Voltar para poder sobrescrever */
	}

	/* Colocar campos no registro */
	reg.codINEP = atoi(campos[0]);

	if (strlen(campos[1]) == 10)
		strncpy(reg.dataAtiv, campos[1], 10);
	else
		strncpy(reg.dataAtiv, DATA_NULA, 10);

	if(strlen(campos[2]) == 2)
		strncpy(reg.uf, campos[2], 2);
	else
		strncpy(reg.uf, UF_NULO, 2);

	reg.indTamEscola = strlen(campos[3]);
	reg.nomeEscola = (char *) malloc(sizeof(char) * (reg.indTamEscola + 1));
	strcpy(reg.nomeEscola, campos[3]);

	reg.indTamMunicipio = strlen(campos[4]);
	reg.municipio = (char *) malloc(sizeof(char) * (reg.indTamMunicipio + 1));
	strcpy(reg.municipio, campos[4]);

	reg.indTamPrestadora = strlen(campos[5]);
	reg.prestadora = (char *) malloc(sizeof(char) * (reg.indTamPrestadora + 1));
	strcpy(reg.prestadora, campos[5]);

	/* Escrever registro e adicionar chave na árvore-B */
	escreverRegistro(&reg, arquivoBinario);
	inserirChave(reg.codINEP, i, arquivoIndices);
	destruirRegistro(&reg);

	cab.status = VERDADEIRO;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Registro inserido com sucesso.\n");
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 7 */
void atualizarRegistro(int RRN, char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função atualiza no arquivo binário 'nomeArquivoDAT' o registro de RRN 'RRN'.
	* Os campos para este registro são passados de forma ordenada (0-5) em 'campos'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;

	/* Validação dos campos */
	if(strlen(campos[1]) != 10 && (campos[1][0] != '0' || campos[1][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[2]) != 2 && (campos[2][0] != '0' || campos[2][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[3]) + strlen(campos[4]) + strlen(campos[5]) > TAMANHO_REGISTRO - sizeof(int) * 4 - sizeof(char) * 12){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	arquivoBinario = fopen(nomeArquivoDAT, "r+b");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "r+b");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cab.status = FALSO;
	escreverCabecalho(arquivoBinario);

	if (fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cabIndice.status = FALSO;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	/* Remover chave antiga da árvore-B */
	removerChave(reg.codINEP, arquivoIndices);
	destruirRegistro(&reg);

	/* Voltar para poder reescrever */
	fseek(arquivoBinario, -1 * TAMANHO_REGISTRO, SEEK_CUR);

	/* Colocar campos no registro */
	reg.codINEP = atoi(campos[0]);

	if (strlen(campos[1]) == 10)
		strncpy(reg.dataAtiv, campos[1], 10);
	else
		strncpy(reg.dataAtiv, DATA_NULA, 10);

	if(strlen(campos[2]) == 2)
		strncpy(reg.uf, campos[2], 2);
	else
		strncpy(reg.uf, UF_NULO, 2);

	reg.indTamEscola = strlen(campos[3]);
	reg.nomeEscola = (char *) malloc(sizeof(char) * (reg.indTamEscola + 1));
	strcpy(reg.nomeEscola, campos[3]);

	reg.indTamMunicipio = strlen(campos[4]);
	reg.municipio = (char *) malloc(sizeof(char) * (reg.indTamMunicipio + 1));
	strcpy(reg.municipio, campos[4]);

	reg.indTamPrestadora = strlen(campos[5]);
	reg.prestadora = (char *) malloc(sizeof(char) * (reg.indTamPrestadora + 1));
	strcpy(reg.prestadora, campos[5]);

	/* Reescrever registro e adicionar nova chave na árvore-B */
	escreverRegistro(&reg, arquivoBinario);
	inserirChave(reg.codINEP, RRN, arquivoIndices);
	destruirRegistro(&reg);

	cab.status = VERDADEIRO;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Registro alterado com sucesso.\n");
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 8 */
void desfragmentarBinario(char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função realiza a compactação eficiente (desfragmentação) do arquivo binário 'nomeArquivoDAT'.
	*
	* Note que ela vai criar um arquivo temporário com o nome ".'nomearquivoDAT'.tmp",
	*   ou seja, se 'nomeArquivoDAT' for igual a "bin.dat", ele vai criar um arquivo
	*   ".bin.dat.tmp" que será usado como auxiliar para desfragmentação.
	*/
	FILE *arquivoBinario, *arquivoBinarioTemporario, *arquivoIndices;
	char *nomeArquivoTemporarioDAT, *nomeArquivoTemporarioIndices;
	REGISTRO reg;
	int aux, i;

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	nomeArquivoTemporarioDAT = (char *) malloc(sizeof(char) * (strlen(nomeArquivoDAT) + 6));
	nomeArquivoTemporarioIndices = (char *) malloc(sizeof(char) * (strlen(nomeArquivoIndices) + 6));

	/* Designar nomes dos arquivos temporários adicionando "." no começo e ".tmp" ao final dos nomes originais */
	nomeArquivoTemporarioDAT[0] = '.';
	strcpy(nomeArquivoTemporarioDAT + sizeof(char), nomeArquivoDAT);
	strcat(nomeArquivoTemporarioDAT, ".tmp");
	nomeArquivoTemporarioIndices[0] = '.';
	strcpy(nomeArquivoTemporarioIndices + sizeof(char), nomeArquivoIndices);
	strcat(nomeArquivoTemporarioIndices, ".tmp");

	/* Cria arquivo de dados temporário */
	arquivoBinarioTemporario = fopen(nomeArquivoTemporarioDAT, "wb");
	if(arquivoBinarioTemporario == NULL){
		printf("Falha no processamento do arquivo.\n");
		free(nomeArquivoTemporarioDAT);
		free(nomeArquivoTemporarioIndices);
		fclose(arquivoBinario);
		return;
	}

	/* Cria arquivo de árvore-B temporário */
	arquivoIndices = fopen(nomeArquivoTemporarioIndices, "w+b");
	if(arquivoIndices == NULL){
		printf("Falha no processamento do arquivo.\n");
		free(nomeArquivoTemporarioDAT);
		free(nomeArquivoTemporarioIndices);
		fclose(arquivoBinario);
		fclose(arquivoBinarioTemporario);
		return;
	}

	cab.status = FALSO;
	cab.topoPilha = FIM_TOPO_PILHA;
	escreverCabecalho(arquivoBinarioTemporario);

	cabIndice.status = FALSO;
	cabIndice.altura = 0;
	cabIndice.noRaiz = cabIndice.ultimoRRN = -1;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	i = 0;
	/* Enquanto houverem registros no arquivo binário original... */
	while((aux = lerRegistro(&reg, arquivoBinario)) != 0) {
		if(aux == -1) /* Registro removido, ignorar */
			continue;
		/* Escrever registro no arquivo temporário e adicionar chave na árvore-B temporária */
		escreverRegistro(&reg, arquivoBinarioTemporario);
		inserirChave(reg.codINEP, i++, arquivoIndices);
		destruirRegistro(&reg);
	}

	cab.status = VERDADEIRO;
	escreverCabecalho(arquivoBinarioTemporario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	fclose(arquivoBinario);
	fclose(arquivoBinarioTemporario);
	fclose(arquivoIndices);

	/* Remove os arquivos originais e renomeia os arquivos temporários de forma que estes adquiram os lugares dos originais. */
	if (remove(nomeArquivoDAT) == 0 && remove(nomeArquivoIndices) == 0 && rename(nomeArquivoTemporarioDAT, nomeArquivoDAT) == 0 && rename(nomeArquivoTemporarioIndices, nomeArquivoIndices) == 0) {
		printf("Arquivo de dados compactado com sucesso.\n");
	} else {
		printf("Falha no processamento do arquivo.\n");
	}

	free(nomeArquivoTemporarioDAT);
	free(nomeArquivoTemporarioIndices);
}


/* FUNCIONALIDADE 9 */
void imprimirPilha(char *nomeArquivoDAT) {
	/*
	* Esta função imprime a pilha de registros removidos do arquivo binário 'nomeArquivoDAT'.
	*/
	FILE *arquivoBinario;
	int aux, contador;
	REGISTRO reg;

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	aux = cab.topoPilha;
	contador = 0;
	/* Enquanto não chegar ao fim da pilha... */
	while (aux != FIM_TOPO_PILHA) {
		/* Imprimir valor atual do RRN na pilha */
		printf("%d ", aux);
		/* Buscar próximo RRN */
		fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * aux, SEEK_SET);
		lerRegistro(&reg, arquivoBinario);
		aux = reg.indTamEscola;
		contador++;
	}
	if(contador < 1)
		printf("Pilha vazia.");
	printf("\n");

	fclose(arquivoBinario);
}


/* Funcionalidades 10 e 11 são equivalentes a 1 e 6 respectivamente */


/* FUNCIONALIDADE 12 */
void recuperarPorChave(int Chave, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função imprime em stdout apenas o registro de chave 'Chave' do arquivo binário 'nomeArquivoDAT'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;
	int RRN;

	arquivoBinario = fopen(nomeArquivoDAT, "rb");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "rb");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}
	iniciarBufferPool();

	/* Busca o RRN na árvore-B */
	RRN = buscaRRN(Chave, arquivoIndices);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	if (RRN < 0 || fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Impressão do registro */
	imprimirRegistro(&reg);

	destruirRegistro(&reg);
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 13 */
void removerRegistroChave(int Chave, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função remove do arquivo binário 'nomeArquivoDAT' o registro de chave 'Chave'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;
	int RRN;

	arquivoBinario = fopen(nomeArquivoDAT, "r+b");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "r+b");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cab.status = FALSO;
	escreverCabecalho(arquivoBinario);

	cabIndice.status = FALSO;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	/* Busca o RRN na árvore-B, já removendo tal chave */
	RRN = removerChave(Chave, arquivoIndices);

	if (RRN < 0 || fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		finalizarBufferPool(nomeArquivoContador, arquivoIndices);
		cabIndice.status = VERDADEIRO;
		escreverCabecalhoIndice(arquivoIndices);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		finalizarBufferPool(nomeArquivoContador, arquivoIndices);
		cabIndice.status = VERDADEIRO;
		escreverCabecalhoIndice(arquivoIndices);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}
	destruirRegistro(&reg);

	/* Voltar para poder reescrever */
	fseek(arquivoBinario, -1 * TAMANHO_REGISTRO, SEEK_CUR);

	/* Atribuir a MASCARA_REMOCAO e o topo da pilha */
	reg.codINEP = MASCARA_REMOCAO;
	reg.indTamEscola = cab.topoPilha;

	/* Reescrever como removido */
	escreverRegistro(&reg, arquivoBinario);

	/* Atualiza o topo da pilha com o RRN do registro que foi removido */
	cab.status = VERDADEIRO;
	cab.topoPilha = RRN;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Registro removido com sucesso.\n");
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}


/* FUNCIONALIDADE 14 */
void atualizarRegistroChave(int Chave, char **campos, char *nomeArquivoDAT, char *nomeArquivoIndices, char *nomeArquivoContador) {
	/*
	* Esta função atualiza no arquivo binário 'nomeArquivoDAT' o registro de chave 'Chave'.
	* Os campos para este registro são passados de forma ordenada (0-5) em 'campos'.
	*/
	FILE *arquivoBinario, *arquivoIndices;
	REGISTRO reg;
	int RRN;

	/* Validação dos campos */
	if(strlen(campos[1]) != 10 && (campos[1][0] != '0' || campos[1][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[2]) != 2 && (campos[2][0] != '0' || campos[2][1] != '\0')){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
	if(strlen(campos[3]) + strlen(campos[4]) + strlen(campos[5]) > TAMANHO_REGISTRO - sizeof(int) * 4 - sizeof(char) * 12){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	arquivoBinario = fopen(nomeArquivoDAT, "r+b");
	if (arquivoBinario == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	lerCabecalho(arquivoBinario);
	if(cab.status != VERDADEIRO){
		/* O status indica que o arquivo está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	arquivoIndices = fopen(nomeArquivoIndices, "r+b");
	if (arquivoIndices == NULL) {
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		return;
	}

	lerCabecalhoIndice(arquivoIndices);
	if(cabIndice.status != VERDADEIRO){
		/* O status indica que o arquivo da árvore-B está inconsistente */
		printf("Falha no processamento do arquivo.\n");
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	cabIndice.status = FALSO;
	escreverCabecalhoIndice(arquivoIndices);
	iniciarBufferPool();

	/* Busca o RRN na árvore-B, já removendo tal chave (antiga) */
	RRN = removerChave(Chave, arquivoIndices);

	if (RRN < 0 || fseek(arquivoBinario, TAMANHO_CABECALHO + TAMANHO_REGISTRO * RRN, SEEK_SET) != 0) {
		/* Se a operação 'fseek' falhar, então o registro não existe */
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		finalizarBufferPool(nomeArquivoContador, arquivoIndices);
		cabIndice.status = VERDADEIRO;
		escreverCabecalhoIndice(arquivoIndices);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}

	/* Leitura do registro */
	if (lerRegistro(&reg, arquivoBinario) != 1) {
		printf("Registro inexistente.\n");
		cab.status = VERDADEIRO;
		escreverCabecalho(arquivoBinario);
		finalizarBufferPool(nomeArquivoContador, arquivoIndices);
		cabIndice.status = VERDADEIRO;
		escreverCabecalhoIndice(arquivoIndices);
		fclose(arquivoBinario);
		fclose(arquivoIndices);
		return;
	}
	destruirRegistro(&reg);

	/* Voltar para poder reescrever */
	fseek(arquivoBinario, -1 * TAMANHO_REGISTRO, SEEK_CUR);

	/* Colocar campos no registro */
	reg.codINEP = atoi(campos[0]);

	if (strlen(campos[1]) == 10)
		strncpy(reg.dataAtiv, campos[1], 10);
	else
		strncpy(reg.dataAtiv, DATA_NULA, 10);

	if(strlen(campos[2]) == 2)
		strncpy(reg.uf, campos[2], 2);
	else
		strncpy(reg.uf, UF_NULO, 2);

	reg.indTamEscola = strlen(campos[3]);
	reg.nomeEscola = (char *) malloc(sizeof(char) * (reg.indTamEscola + 1));
	strcpy(reg.nomeEscola, campos[3]);

	reg.indTamMunicipio = strlen(campos[4]);
	reg.municipio = (char *) malloc(sizeof(char) * (reg.indTamMunicipio + 1));
	strcpy(reg.municipio, campos[4]);

	reg.indTamPrestadora = strlen(campos[5]);
	reg.prestadora = (char *) malloc(sizeof(char) * (reg.indTamPrestadora + 1));
	strcpy(reg.prestadora, campos[5]);

	/* Reescrever registro e adicionar nova chave na árvore-B */
	escreverRegistro(&reg, arquivoBinario);
	inserirChave(reg.codINEP, RRN, arquivoIndices);
	destruirRegistro(&reg);

	cab.status = VERDADEIRO;
	escreverCabecalho(arquivoBinario);

	finalizarBufferPool(nomeArquivoContador, arquivoIndices);
	cabIndice.status = VERDADEIRO;
	escreverCabecalhoIndice(arquivoIndices);

	printf("Registro alterado com sucesso.\n");
	fclose(arquivoBinario);
	fclose(arquivoIndices);
}



/* == Funções úteis == */


int lerRegistro(REGISTRO *reg, FILE *arquivoDAT) {
	/*
	* Esta função lê um registro de 'arquivoDAT' e atribui em 'reg'.
	*
	* Ela retorna 1 se o registro foi lido com sucesso e é válido,
	*   -1 se foi lido com sucesso mas é um registro removido,
	*   ou 0 em caso de erros/EOF.
	*/
	char auxRegistro[TAMANHO_REGISTRO], *auxPonteiro;
	auxPonteiro = auxRegistro;

	/* Ler registro do disco, verificando caso de erros/EOF */
	if(fread(auxRegistro, TAMANHO_REGISTRO, 1, arquivoDAT) != 1) {
		return 0;
	}

	/* Copiar todos os campos do espaço de memória do registro para 'reg' */
	memcpy(&reg->codINEP, auxPonteiro, sizeof(int));
	auxPonteiro += sizeof(int);

	memcpy(reg->dataAtiv, auxPonteiro, sizeof(char) * 10);
	auxPonteiro += sizeof(char) * 10;

	memcpy(reg->uf, auxPonteiro, sizeof(char) * 2);
	auxPonteiro += sizeof(char) * 2;

	if(reg->codINEP == MASCARA_REMOCAO) {
		/* Caso em que o registro foi removido */
		memcpy(&reg->indTamEscola, reg->dataAtiv, sizeof(int));
		return -1;
	}

	memcpy(&reg->indTamEscola, auxPonteiro, sizeof(int));
	auxPonteiro += sizeof(int);

	reg->nomeEscola = (char *) malloc(sizeof(char) * (reg->indTamEscola + 1));
	memcpy(reg->nomeEscola, auxPonteiro, sizeof(char) * reg->indTamEscola);
	reg->nomeEscola[reg->indTamEscola] = '\0';
	auxPonteiro += sizeof(char) * reg->indTamEscola;

	memcpy(&reg->indTamMunicipio, auxPonteiro, sizeof(int));
	auxPonteiro += sizeof(int);

	reg->municipio = (char *) malloc(sizeof(char) * (reg->indTamMunicipio + 1));
	memcpy(reg->municipio, auxPonteiro, sizeof(char) * reg->indTamMunicipio);
	reg->municipio[reg->indTamMunicipio] = '\0';
	auxPonteiro += sizeof(char) * reg->indTamMunicipio;

	memcpy(&reg->indTamPrestadora, auxPonteiro, sizeof(int));
	auxPonteiro += sizeof(int);

	reg->prestadora = (char *) malloc(sizeof(char) * (reg->indTamPrestadora + 1));
	memcpy(reg->prestadora, auxPonteiro, sizeof(char) * reg->indTamPrestadora);
	reg->prestadora[reg->indTamPrestadora] = '\0';

	return 1;
}

int lerRegistroCSV(FILE *arquivoCSV, REGISTRO *reg) {
	/*
	* Esta função lê do arquivo *.CSV 'arquivoCSV' um registro e atribui seus campos em 'reg'.
	*
	* Ela retorna 1 se o registro foi lido com sucesso e é válido,
	*   -1 se foi lido com sucesso mas é inválido,
	*   ou 0 em caso de erros/EOF.
	*/
	char auxLinha[1024], auxCampos[6][1024]; /* Todos os campos do *.CSV são do tipo string. */
	int i, j, k;

	memset(auxCampos, 0, sizeof(auxCampos));

	/* Ler uma linha de um registro */
	if (fscanf(arquivoCSV, "%1024[^\n]%*c", auxLinha) < 1)
		return 0;

	/* Dessa linha lida, obter campos */
	for(i = j = k = 0; auxLinha[i] != '\0' && j < 6; i++){
		if(auxLinha[i] == ';'){
			k = 0;
			j++;
			continue;
		}
		auxCampos[j][k++] = auxLinha[i];
	}

	if (strlen(auxCampos[2]) < 1) /* Código INEP não pode ser nulo */
		return -1;

	if(strlen(auxCampos[0]) + strlen(auxCampos[3]) + strlen(auxCampos[4]) > TAMANHO_REGISTRO - sizeof(int) * 4 + sizeof(char) * 12) /* Campos inválidos */
		return -1;

	/* Atribuir prestadora */
	reg->indTamPrestadora = strlen(auxCampos[0]);
	reg->prestadora = (char *) malloc(sizeof(char) * (reg->indTamPrestadora + 1));
	strcpy(reg->prestadora, auxCampos[0]);

	/* Atribuir data */
	if (strlen(auxCampos[1]) == 10)
		strncpy(reg->dataAtiv, auxCampos[1], 10);
	else
		strncpy(reg->dataAtiv, DATA_NULA, 10);

	/* Atribuir código INEP */
	reg->codINEP = atoi(auxCampos[2]);

	/* Atribuir escola */
	reg->indTamEscola = strlen(auxCampos[3]);
	reg->nomeEscola = (char *) malloc(sizeof(char) * (reg->indTamEscola + 1));
	strcpy(reg->nomeEscola, auxCampos[3]);

	/* Atribuir município */
	reg->indTamMunicipio = strlen(auxCampos[4]);
	reg->municipio = (char *) malloc(sizeof(char) * (reg->indTamMunicipio + 1));
	strcpy(reg->municipio, auxCampos[4]);

	/* Atribuir UF */
	if (strlen(auxCampos[5]) < 2)
		strncpy(reg->uf, UF_NULO, 2);
	else
		strncpy(reg->uf, auxCampos[5], 2);

	return 1;
}

void escreverRegistro(REGISTRO *reg, FILE *arquivoDAT) {
	/*
	* Esta função escreve em 'arquivoDAT' o registro 'reg'.
	*/
	char auxRegistro[TAMANHO_REGISTRO], *auxPonteiro;
	auxPonteiro = auxRegistro;
	memset(auxRegistro, REGISTRO_NULO, TAMANHO_REGISTRO);

	/* Escrever todos os campos nesse espaço reservado para o registro */
	memcpy(auxPonteiro, &reg->codINEP, sizeof(int));
	auxPonteiro += sizeof(int);

	if(reg->codINEP == MASCARA_REMOCAO){
		/* Caso em que o registro está marcado como removido */
		memcpy(auxPonteiro, &reg->indTamEscola, sizeof(int));
		fwrite(auxRegistro, TAMANHO_REGISTRO, 1, arquivoDAT);
		return;
	}

	memcpy(auxPonteiro, reg->dataAtiv, sizeof(char) * 10);
	auxPonteiro += sizeof(char) * 10;

	memcpy(auxPonteiro, reg->uf, sizeof(char) * 2);
	auxPonteiro += sizeof(char) * 2;

	memcpy(auxPonteiro, &reg->indTamEscola, sizeof(int));
	auxPonteiro += sizeof(int);

	memcpy(auxPonteiro, reg->nomeEscola, sizeof(char) * reg->indTamEscola);
	auxPonteiro += sizeof(char) * reg->indTamEscola;

	memcpy(auxPonteiro, &reg->indTamMunicipio, sizeof(int));
	auxPonteiro += sizeof(int);

	memcpy(auxPonteiro, reg->municipio, sizeof(char) * reg->indTamMunicipio);
	auxPonteiro += sizeof(char) * reg->indTamMunicipio;

	memcpy(auxPonteiro, &reg->indTamPrestadora, sizeof(int));
	auxPonteiro += sizeof(int);

	memcpy(auxPonteiro, reg->prestadora, sizeof(char) * reg->indTamPrestadora);

	/* Escrever em disco */
	fwrite(auxRegistro, TAMANHO_REGISTRO, 1, arquivoDAT);
}


void imprimirRegistro(REGISTRO *reg) {
	/*
	* Esta função imprime em stdout todos os campos do registro 'reg'.
	*/
	printf("%d ", reg->codINEP);

	if(reg->dataAtiv[0] != '\0')
		printf("%.10s ", reg->dataAtiv);
	else
		printf("%s ", "0000000000");

	if(reg->uf[0] != '\0')
		printf("%.2s ", reg->uf);
	else
		printf("%s ", "00");

	printf("%d ", reg->indTamEscola);

	printf("%s ", reg->nomeEscola);

	printf("%d ", reg->indTamMunicipio);

	printf("%s ", reg->municipio);

	printf("%d ", reg->indTamPrestadora);

	printf("%s\n", reg->prestadora);
}

void destruirRegistro(REGISTRO *reg) {
	/*
	* Esta função limpa da memória dinâmica (HEAP) os campos de tamanho
	*   variável que foram alocados em 'reg'.
	*/
	free(reg->municipio);
	free(reg->nomeEscola);
	free(reg->prestadora);
}


