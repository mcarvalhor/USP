#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cabecalho.h>



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


#define TAMANHO_PAGINA 116
#define PAGINA_NULO -1
#define ORDEM 10



/* == Declaração da estrutura de uma página == */

typedef struct {
	int	N,
		P[ORDEM],
		C[ORDEM - 1],
		PR[ORDEM - 1];
	int	noP;
	char
		changed,
		secondChance;
} PAGINA_NO;



/* Buffer-pool com 4 posições + uma posição reservada para nó raiz */

PAGINA_NO Buffer[4];
PAGINA_NO BufferRaiz;

/* Contadores de "page fault" e "page hit" */

int	PageFault,
	PageHit;



/* Funções usadas pela árvore-B */

void iniciarBufferPool();
void finalizarBufferPool(char *, FILE *);
int FIFOShift(FILE *);
PAGINA_NO lerPagina(int, FILE *);
void escreverPagina(PAGINA_NO, FILE *);

int inserirChaveRecursao(int, int, int, int *, int *, PAGINA_NO, FILE *);
int removerChaveRecursao(int, int *, int, int, int *, int *, int, int *, int *, int *, int *, PAGINA_NO, FILE *);



/* Implementação da Árvore-B */

int buscaRRN(int chave, FILE *arquivoIndices) {
	/*
	* Esta função retorna o RRN (do arquivo de dados) para uma chave de busca 'chave'.
	* Em caso de chave não encontrada, retorna -1.
	*/
	PAGINA_NO Aux;
	int i, altura;

	/*Aux = lerPagina(cabIndice.noRaiz, arquivoIndices);*/
	i = 0;
	Aux.P[0] = cabIndice.noRaiz;
	altura = cabIndice.altura;
	do { /* Enquanto tiver filhos... */
		Aux = lerPagina(Aux.P[i], arquivoIndices);
		for(i = 0; i < Aux.N; i++) { /* Percorrer cada chave no nó */
			if(Aux.C[i] > chave) { /* Verificar onde se posiciona a chave (busca dentro do nó) */
				break;
			} else if(Aux.C[i] == chave){
				return Aux.PR[i]; /* Encontrou */
			}
		}
		altura--;
	} while(altura >= 0);

	return -1; /* Não encontrou a chave na árvore-B */
}

void inserirChave(int chave, int RRN, FILE *arquivoIndices) {
	/*
	* Esta função insere a chave de busca 'chave', junto com o RRN 'RRN' na árvore-B.
	*/
	int P, C, PR;
	PAGINA_NO Aux;

	if(cabIndice.altura < 1) {
		/* Caso em que não há raiz criada ainda */
		memset(&Aux, PAGINA_NULO, sizeof(PAGINA_NO));
		Aux.noP = 0;
		Aux.N = 1;
		Aux.C[0] = chave;
		Aux.PR[0] = RRN;
		cabIndice.altura = 1;
		cabIndice.noRaiz = cabIndice.ultimoRRN = 0;
		escreverPagina(Aux, arquivoIndices);
	} else if((P = inserirChaveRecursao(chave, RRN, cabIndice.altura - 1, &C, &PR, lerPagina(cabIndice.noRaiz, arquivoIndices), arquivoIndices)) >= 0) {
		/* Caso em que é necessário alocar uma nova raiz (chave promovida abaixo) */
		memset(&Aux, PAGINA_NULO, sizeof(PAGINA_NO));
		Aux.noP = ++cabIndice.ultimoRRN;
		Aux.N = 1;
		Aux.C[0] = C;
		Aux.PR[0] = PR;
		Aux.P[0] = cabIndice.noRaiz;
		Aux.P[1] = P;
		cabIndice.altura++;
		cabIndice.noRaiz = Aux.noP;
		escreverPagina(Aux, arquivoIndices);
	}
}

int removerChave(int chave, FILE *arquivoIndices) {
	/*
	* Esta função remove a chave de busca 'chave' da árvore-B.
	*
	* Ela retorna o RRN da chave que acaba de ser removida.
	*/
	PAGINA_NO raiz;
	int i, j = 97, Aux;

	if(cabIndice.altura < 1)
		return -1;

	raiz = lerPagina(cabIndice.noRaiz, arquivoIndices);

	for(i = 0; i < raiz.N; i++) { /* Percorrer cada chave no nó */
		if(raiz.C[i] >= chave) /* Verificar onde se posiciona a chave (busca dentro do nó) */
			break;
	}

	if(cabIndice.altura < 2 && i < raiz.N && raiz.C[i] == chave) {
		/* Caso em que só há uma página (raiz) e a chave está nela */
		Aux = 3;
		j = raiz.PR[i];
	}else if(cabIndice.altura < 2 && (i >= raiz.N || raiz.C[i] != chave)) {
		/* Caso em que só há uma página (raiz) e a chave não está nela */
		Aux = j = -1;
	} else if(i < raiz.N && raiz.C[i] == chave) {
		/* Caso em que há mais de uma página, mas a chave se encontra na raiz */
		Aux = removerChaveRecursao(chave, &j, cabIndice.altura - 1, (i > 0) ? raiz.P[i - 1] : -1, (i > 0) ? &raiz.C[i - 1] : NULL, (i > 0) ? &raiz.PR[i - 1] : NULL, raiz.P[i + 1], &raiz.C[i], &raiz.PR[i], &raiz.C[i], &raiz.PR[i], lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
		if(Aux == 0)
			escreverPagina(raiz, arquivoIndices);
	} else if(i < raiz.N) {
		/* Caso em que há mais de uma página, e a chave não se encontra na raiz */
		Aux = removerChaveRecursao(chave, &j, cabIndice.altura - 1, (i > 0) ? raiz.P[i - 1] : -1, (i > 0) ? &raiz.C[i - 1] : NULL, (i > 0) ? &raiz.PR[i - 1] : NULL, raiz.P[i + 1], &raiz.C[i], &raiz.PR[i], NULL, NULL, lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
	} else {
		/* Caso em que há mais de uma página, e a chave não se encontra na raiz (maior filho) */
		Aux = removerChaveRecursao(chave, &j, cabIndice.altura - 1, raiz.P[i - 1], &raiz.C[i - 1], &raiz.PR[i - 1], -1, NULL, NULL, NULL, NULL, lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
	}
	if(Aux == 0 || Aux == -1) {
		return j;
	} else if(Aux == 1) {
		escreverPagina(raiz, arquivoIndices);
		return j;
	} else if(Aux == 2) {
		i--;
	}

	if(raiz.N < 2) {
		/* Caso em que a altura da árvore diminui porque houve concatenação na raiz */
		cabIndice.noRaiz = raiz.P[i + 1];
		cabIndice.altura--;
		lerPagina(cabIndice.noRaiz, arquivoIndices); /* Apenas para forçar colocação da página raiz no buffer-pool */
		return j;
	}

	/* Caso em que houve concatenação na raiz sem diminuir a altura da árvore */
	memmove(&raiz.C[i], &raiz.C[i + 1], sizeof(int) * (ORDEM - 2 - i));
	memmove(&raiz.PR[i], &raiz.PR[i + 1], sizeof(int) * (ORDEM - 2 - i));
	memmove(&raiz.P[i], &raiz.P[i + 1], sizeof(int) * (ORDEM - 1 - i));
	raiz.N--;
	escreverPagina(raiz, arquivoIndices);

	return j;
}



/* Implementação das funções usadas pela árvore-B */


void iniciarBufferPool() {
	/*
	* Esta função inicia o buffer-pool, colocando todas as páginas lá presentes como nulas.
	*/
	memset(&Buffer, -1, sizeof(PAGINA_NO) * 4);
	memset(&BufferRaiz, -1, sizeof(PAGINA_NO));
	PageFault = PageHit = 0;
}

void finalizarBufferPool(char *nomeArquivoContadorPageFaultHit, FILE *arquivoIndices) {
	/*
	* Esta função finaliza o buffer-pool.
	* Se existirem páginas que precisam ser escritas em disco, estas são escritas.
	* Ela também escreve no arquivo de texto o número de "page fault" e "page hit".
	*/
	FILE *contadorPageFaultHit;
	char aux[TAMANHO_PAGINA];
	int i, j;

	/* Escrever possíveis páginas que estavam modificadas no buffer no disco */
	for(i = 0; i < 4; i++) {
		if(Buffer[i].noP < 0 || Buffer[i].changed == 0)
			continue;
		memcpy(aux, &Buffer[i].N, sizeof(int));
		for(j = 0; j < ORDEM - 1; j++) {
			memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 2), &Buffer[i].C[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 3), &Buffer[i].PR[j], sizeof(int));
		}
		memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
		if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * Buffer[i].noP, SEEK_SET) == 0)
			fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
	}

	/* Escrever nó raiz, se for o caso */
	if(BufferRaiz.noP >= 0 && BufferRaiz.changed) {
		memcpy(aux, &BufferRaiz.N, sizeof(int));
		for(j = 0; j < ORDEM - 1; j++) {
			memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 2), &BufferRaiz.C[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 3), &BufferRaiz.PR[j], sizeof(int));
		}
		memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
		if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * BufferRaiz.noP, SEEK_SET) == 0)
			fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
	}

	/* Acrescentar contagem de "page fault" e "page hit" */
	contadorPageFaultHit = fopen(nomeArquivoContadorPageFaultHit, "a");
	if(contadorPageFaultHit == NULL)
		return;
	fprintf(contadorPageFaultHit, "Page fault: %d; Page hit: %d.\n", PageFault, PageHit);
	fclose(contadorPageFaultHit);
}

int FIFOShift(FILE *arquivoIndices) {
	/*
	* Esta função cuida do Shifting da FIFO e lida com o Second-chance Algorithm.
	*/
	char aux[TAMANHO_PAGINA];
	int i, j;
	for(i = 0; i < 4; i++)
		if(Buffer[i].noP < 0) /* Caso em que o Buffer ainda tem espaço sobrando */
			return i;
	for(i = 0; i < 4; i++) { /* Caso em que o Buffer está cheio e é necessário aplicar a política de substituição: SCA */
		if(Buffer[i].secondChance) {
			if(Buffer[i].changed) {
				memcpy(aux, &Buffer[i].N, sizeof(int));
				for(j = 0; j < ORDEM - 1; j++) {
					memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
					memcpy(aux + sizeof(int) * (j * 3 + 2), &Buffer[i].C[j], sizeof(int));
					memcpy(aux + sizeof(int) * (j * 3 + 3), &Buffer[i].PR[j], sizeof(int));
				}
				memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
				if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * Buffer[i].noP, SEEK_SET) == 0)
					fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
			}
			memmove(&Buffer[i], &Buffer[i + 1], sizeof(PAGINA_NO) * (3 - i));
			return 3;
		} else {
			Buffer[i].secondChance = 1;
		}
	}
	i = 0;
	if(Buffer[i].changed) { /* Caso em que percorreu todos: volta para o início da fila */
		memcpy(aux, &Buffer[i].N, sizeof(int));
		for(j = 0; j < ORDEM - 1; j++) {
			memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 2), &Buffer[i].C[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 3), &Buffer[i].PR[j], sizeof(int));
		}
		memcpy(aux + sizeof(int) * (j * 3 + 1), &Buffer[i].P[j], sizeof(int));
		if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * Buffer[i].noP, SEEK_SET) == 0)
			fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
	}
	memmove(&Buffer[i], &Buffer[i + 1], sizeof(PAGINA_NO) * (3 - i));
	return 3;
}

PAGINA_NO lerPagina(int P, FILE *arquivoIndices) {
	/*
	* Esta função retorna uma página de índice relativo 'P' da árvore-B.
	*
	* Se a página já estiver no buffer-pool, não há acessos a disco.
	* Caso contrário, a página entra para o buffer-pool (substituindo outra, se for o caso, de acordo
	*   com a política de substituição), fazendo acessos a disco.
	*/
	char aux[TAMANHO_PAGINA];
	PAGINA_NO destino;
	int i, j;

	memset(&destino, PAGINA_NULO, sizeof(PAGINA_NO));

	if(P < 0)
		return destino;

	if(BufferRaiz.noP == P) { /* Foi pedido o nó raiz. Ele está no Buffer? */
		PageHit++;
		return BufferRaiz;
	}

	for(i = 0; i < 4; i++) { /* Este nó está no Buffer? */
		if(Buffer[i].noP == P) {
			PageHit++;
			Buffer[i].secondChance = 0;
			return Buffer[i];
		}
	}

	/* Não está no Buffer: carregar do disco */
	if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * P, SEEK_SET) != 0)
		return destino;

	if(fread(aux, TAMANHO_PAGINA, 1, arquivoIndices) != 1)
		return destino;

	PageFault++;

	destino.noP = P;
	destino.changed = 0;
	destino.secondChance = 1;
	memcpy(&destino.N, aux, sizeof(int));
	for(i = 0; i < ORDEM - 1; i++) {
		memcpy(&destino.P[i], aux + sizeof(int) * (i * 3 + 1), sizeof(int));
		memcpy(&destino.C[i], aux + sizeof(int) * (i * 3 + 2), sizeof(int));
		memcpy(&destino.PR[i], aux + sizeof(int) * (i * 3 + 3), sizeof(int));
	}
	memcpy(&destino.P[i], aux + sizeof(int) * (i * 3 + 1), sizeof(int));

	if(cabIndice.noRaiz == P) { /* Se for um novo nó raiz */
		if(BufferRaiz.noP >=0 && BufferRaiz.changed) { /* Salvar? */
			memcpy(aux, &BufferRaiz.N, sizeof(int));
			for(j = 0; j < ORDEM - 1; j++) {
				memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
				memcpy(aux + sizeof(int) * (j * 3 + 2), &BufferRaiz.C[j], sizeof(int));
				memcpy(aux + sizeof(int) * (j * 3 + 3), &BufferRaiz.PR[j], sizeof(int));
			}
			memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
			if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * BufferRaiz.noP, SEEK_SET) == 0)
				fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
		}
		memcpy(&BufferRaiz, &destino, sizeof(PAGINA_NO));
		return destino;
	}

	memcpy(&Buffer[FIFOShift(arquivoIndices)], &destino, sizeof(PAGINA_NO));
	return destino;
}

void escreverPagina(PAGINA_NO origem, FILE *arquivoIndices) {
	/*
	* Esta função salva uma página na árvore-B.
	*
	* Se a página já estiver no buffer-pool, não há acessos a disco.
	* Caso contrário, a página entra para o buffer-pool (substituindo outra, se for o caso, de acordo
	*   com a política de substituição), fazendo acessos a disco.
	*/
	char aux[TAMANHO_PAGINA];
	int i, j;

	if(origem.noP < 0)
		return;

	origem.changed = 1;
	origem.secondChance = 1;

	if(BufferRaiz.noP == origem.noP) { /* Foi modificado o nó raiz. Ele está no Buffer? */
		PageHit++;
		memcpy(&BufferRaiz, &origem, sizeof(PAGINA_NO));
		return;
	}

	for(i = 0; i < 4; i++) { /* Este nó está no Buffer? */
		if(Buffer[i].noP == origem.noP) {
			PageHit++;
			memcpy(&Buffer[i], &origem, sizeof(PAGINA_NO));
			Buffer[i].secondChance = 0;
			return;
		}
	}

	/* Não está no Buffer: colocar para salvar posteriormente, ou criar imediatamente em disco */
	PageFault++;

	if(origem.noP == cabIndice.ultimoRRN) { /* Página recém-criada: é necessário gravar em disco */
		memcpy(aux, &origem.N, sizeof(int));
		for(j = 0; j < ORDEM - 1; j++) {
			memcpy(aux + sizeof(int) * (j * 3 + 1), &origem.P[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 2), &origem.C[j], sizeof(int));
			memcpy(aux + sizeof(int) * (j * 3 + 3), &origem.PR[j], sizeof(int));
		}
		memcpy(aux + sizeof(int) * (j * 3 + 1), &origem.P[j], sizeof(int));
		if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * origem.noP, SEEK_SET) == 0)
			fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
		origem.changed = 0;
	}

	if(cabIndice.noRaiz == origem.noP) { /* Se for um novo nó raiz */
		if(BufferRaiz.noP >=0 && BufferRaiz.changed) { /* Salvar? */
			memcpy(aux, &BufferRaiz.N, sizeof(int));
			for(j = 0; j < ORDEM - 1; j++) {
				memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
				memcpy(aux + sizeof(int) * (j * 3 + 2), &BufferRaiz.C[j], sizeof(int));
				memcpy(aux + sizeof(int) * (j * 3 + 3), &BufferRaiz.PR[j], sizeof(int));
			}
			memcpy(aux + sizeof(int) * (j * 3 + 1), &BufferRaiz.P[j], sizeof(int));
			if(fseek(arquivoIndices, TAMANHO_CABECALHO_INDICE + TAMANHO_PAGINA * BufferRaiz.noP, SEEK_SET) == 0)
				fwrite(aux, TAMANHO_PAGINA, 1, arquivoIndices);
		}
		memcpy(&BufferRaiz, &origem, sizeof(PAGINA_NO));
		return;
	}

	memcpy(&Buffer[FIFOShift(arquivoIndices)], &origem, sizeof(PAGINA_NO));
}


int inserirChaveRecursao(int chave, int RRN, int altura, int *CSplit, int *PRSplit, PAGINA_NO raiz, FILE *arquivoIndices) {
	/*
	* Esta é a recursão que faz a inserção de uma chave na árvore-B.
	* Ela já lida com Split quando necessário, e preza pelo menor número de acessos
	*   a disco que puder.
	*/
	int i, P, C, PR;
	PAGINA_NO Aux;

	C = chave;
	PR = RRN;
	P = PAGINA_NULO;

	for(i = 0; i < raiz.N; i++) { /* Percorrer cada chave no nó */
		if(raiz.C[i] > chave) /* Verificar onde se posiciona a chave (busca dentro do nó) */
			break;
	}

	/* Chamar recursão (se não for nó folha), e verificar se houve chave promovida. */
	if(altura > 0 && (P=inserirChaveRecursao(chave, RRN, altura - 1, &C, &PR, lerPagina(raiz.P[i], arquivoIndices), arquivoIndices)) < 0) {
		return -1;
	}

	/* Aparentemente chegou uma chave para ser adicionada nesse nó. */
	if(raiz.N < ORDEM - 1) {
		/* Tem espaço pra ela nesse nó. Perfeito, só adiciona. */
		memmove(&raiz.C[i + 1], &raiz.C[i], sizeof(int) * (ORDEM - 2 - i));
		memmove(&raiz.PR[i + 1], &raiz.PR[i], sizeof(int) * (ORDEM - 2 - i));
		memmove(&raiz.P[i + 2], &raiz.P[i + 1], sizeof(int) * (ORDEM - 2 - i));
		raiz.C[i] = C;
		raiz.PR[i] = PR;
		raiz.P[i + 1] = P;
		raiz.N++;
		escreverPagina(raiz, arquivoIndices);
		return -1;
	}

	/* Eita! Não tem espaço pra essa chave aqui. Vamos ter que fazer Split e promover alguém. */
	memset(&Aux, PAGINA_NULO, sizeof(PAGINA_NO));
	Aux.noP = ++cabIndice.ultimoRRN;
	Aux.N = (ORDEM - 1) / 2;
	raiz.N = ORDEM / 2;
	if(i > ORDEM/2) { /* Caso em que a chave promovida anteriormente está no primeiro nó */
		*CSplit = raiz.C[ORDEM/2];
		*PRSplit = raiz.PR[ORDEM/2];
		memcpy(&Aux.C, &raiz.C[ORDEM/2 + 1], sizeof(int) * (i - ORDEM/2 - 1));
		memcpy(&Aux.PR, &raiz.PR[ORDEM/2 + 1], sizeof(int) * (i - ORDEM/2 - 1));
		memcpy(&Aux.P, &raiz.P[ORDEM/2 + 1], sizeof(int) * (i - ORDEM/2));
		memcpy(&Aux.C[i - ORDEM/2], &raiz.C[i], sizeof(int) * (ORDEM - 1 - i));
		memcpy(&Aux.PR[i - ORDEM/2], &raiz.PR[i], sizeof(int) * (ORDEM - 1 - i));
		memcpy(&Aux.P[i - ORDEM/2 + 1], &raiz.P[i + 1], sizeof(int) * (ORDEM - 1 - i));
		Aux.C[i - ORDEM/2 - 1] = C;
		Aux.PR[i - ORDEM/2 - 1] = PR;
		Aux.P[i - ORDEM/2] = P;
	} else if(i < ORDEM/2) { /* Caso em que a chave promovida anteriormente está no segundo nó */
		*CSplit = raiz.C[ORDEM/2 - 1];
		*PRSplit = raiz.PR[ORDEM/2 - 1];
		memcpy(&Aux.C, &raiz.C[ORDEM/2], sizeof(int) * (ORDEM - ORDEM/2 - 1));
		memcpy(&Aux.PR, &raiz.PR[ORDEM/2], sizeof(int) * (ORDEM - ORDEM/2 - 1));
		memcpy(&Aux.P, &raiz.P[ORDEM/2], sizeof(int) * (ORDEM - ORDEM/2));
		memmove(&raiz.C[i + 1], &raiz.C[i], sizeof(int) * (ORDEM/2 - 1 - i));
		memmove(&raiz.PR[i + 1], &raiz.PR[i], sizeof(int) * (ORDEM/2 - 1 - i));
		memmove(&raiz.P[i + 2], &raiz.P[i + 1], sizeof(int) * (ORDEM/2 - 1 - i));
		raiz.C[i] = C;
		raiz.PR[i] = PR;
		raiz.P[i + 1] = P;
	} else { /* Caso em que a chave promovida anteriormente está no centro */
		*CSplit = C;
		*PRSplit = PR;
		memcpy(&Aux.C, &raiz.C[ORDEM/2], sizeof(int) * (ORDEM - ORDEM/2 - 1));
		memcpy(&Aux.PR, &raiz.PR[ORDEM/2], sizeof(int) * (ORDEM - ORDEM/2 - 1));
		memcpy(&Aux.P[1], &raiz.P[ORDEM/2 + 1], sizeof(int) * (ORDEM - ORDEM/2 - 1));
		Aux.P[0] = P;
	}
	memset(&raiz.C[ORDEM/2], PAGINA_NULO, sizeof(int) * (ORDEM - ORDEM/2 - 1));
	memset(&raiz.PR[ORDEM/2], PAGINA_NULO, sizeof(int) * (ORDEM - ORDEM/2 - 1));
	memset(&raiz.P[ORDEM/2 + 1], PAGINA_NULO, sizeof(int) * (ORDEM - ORDEM/2 - 1));
	escreverPagina(Aux, arquivoIndices);
	escreverPagina(raiz, arquivoIndices);
	return Aux.noP;
}

int removerChaveRecursao(int chave, int *RRN, int altura, int PEsq, int *CEsqConcat, int *PREsqConcat, int PDir, int *CDirConcat, int *PRDirConcat, int *trocaC, int *trocaPR, PAGINA_NO raiz, FILE *arquivoIndices) {
	/*
	* Esta é a recursão que faz a remoção de uma chave na árvore-B.
	* Ela já lida com Concatenação quando necessário, e preza pelo menor número de acessos
	*   a disco que puder.
	*/
	PAGINA_NO AuxL, AuxR;
	int i, j, Aux;

	for(i = 0; i < raiz.N; i++) { /* Percorrer cada chave no nó */
		if(raiz.C[i] >= chave) /* Verificar onde se posiciona a chave (busca dentro do nó) */
			break;
	}

	if(altura > 1) {
		/* Caso em que a recursão não está em nó-folha */
		if(i < raiz.N && raiz.C[i] == chave) {
			Aux = removerChaveRecursao(chave, RRN, altura - 1, (i > 0) ? raiz.P[i - 1] : -1, (i > 0) ? &raiz.C[i - 1] : NULL, (i > 0) ? &raiz.PR[i - 1] : NULL, raiz.P[i + 1], &raiz.C[i], &raiz.PR[i], &raiz.C[i], &raiz.PR[i], lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
			if(Aux == 0)
				escreverPagina(raiz, arquivoIndices);
		} else if(i < raiz.N) {
			Aux = removerChaveRecursao(chave, RRN, altura - 1, (i > 0) ? raiz.P[i - 1] : -1, (i > 0) ? &raiz.C[i - 1] : NULL, (i > 0) ? &raiz.PR[i - 1] : NULL, raiz.P[i + 1], &raiz.C[i], &raiz.PR[i], trocaC, trocaPR, lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
		} else {
			Aux = removerChaveRecursao(chave, RRN, altura - 1, raiz.P[i - 1], &raiz.C[i - 1], &raiz.PR[i - 1], -1, NULL, NULL, trocaC, trocaPR, lerPagina(raiz.P[i], arquivoIndices), arquivoIndices);
		}
		if(Aux == 0 || Aux == -1) {
			return Aux;
		} else if(Aux == 1) {
			escreverPagina(raiz, arquivoIndices);
			return 0;
		} else if(Aux == 2) {
			i--;
		}
	}else if(trocaC != NULL) {
		/* Caso em que a chave não está em nó-folha */
		i = raiz.N - 1;
		Aux = *trocaC;
		*trocaC = raiz.C[i];
		raiz.C[i] = Aux;
		Aux = *trocaPR;
		*trocaPR = raiz.PR[i];
		*RRN = raiz.PR[i] = Aux;
	} else if(i >= raiz.N || raiz.C[i]!= chave) {
		/* Caso "chave não encontrada" */
		*RRN = -1;
		return -1;
	} else {
		/* Salvar o RRN */
		*RRN = raiz.PR[i];
	}

	/* Vamos remover a chave em i */
	memmove(&raiz.C[i], &raiz.C[i + 1], sizeof(int) * (ORDEM - 2 - i));
	memmove(&raiz.PR[i], &raiz.PR[i + 1], sizeof(int) * (ORDEM - 2 - i));
	memmove(&raiz.P[i], &raiz.P[i + 1], sizeof(int) * (ORDEM - 1 - i));
	raiz.N--;

	if(raiz.N >= ORDEM / 2 - 1) {
		/* Caso em que não houve underflow: perfeito! */
		escreverPagina(raiz, arquivoIndices);
		return 0;
	}

	/* Houve underflow. Precisamos tratar isso... */
	if(PEsq >= 0 && (AuxL = lerPagina(PEsq, arquivoIndices)).N >= ORDEM/2) {
		/* 1) Tentar redistribuir com o irmão da esquerda */
		j = (AuxL.N + raiz.N + 1) / 2;
		memmove(&raiz.C[AuxL.N - j], raiz.C, sizeof(int) * (ORDEM / 2 - 2));
		memmove(&raiz.PR[AuxL.N - j], raiz.PR, sizeof(int) * (ORDEM / 2 - 2));
		memmove(&raiz.P[AuxL.N - j], raiz.P, sizeof(int) * (ORDEM / 2 - 1));
		memcpy(&raiz.C[AuxL.N - j - 1], CEsqConcat, sizeof(int));
		memcpy(&raiz.PR[AuxL.N - j - 1], PREsqConcat, sizeof(int));
		memcpy(raiz.C, &AuxL.C[j + 1], sizeof(int) * (AuxL.N - j - 1));
		memcpy(raiz.PR, &AuxL.PR[j + 1], sizeof(int) * (AuxL.N - j - 1));
		memcpy(raiz.P, &AuxL.P[j], sizeof(int) * (AuxL.N - j));
		*CEsqConcat = AuxL.C[j];
		*PREsqConcat = AuxL.PR[j];
		raiz.N += AuxL.N - j;
		AuxL.N = j;
		escreverPagina(AuxL, arquivoIndices);
		escreverPagina(raiz, arquivoIndices);
		return 1;
	}else if(PDir >= 0 && (AuxR = lerPagina(PDir, arquivoIndices)).N >= ORDEM/2) {
		/* Não deu: 2) Tentar redistribuir com o irmão da direita */
		j = (AuxR.N + raiz.N + 1) / 2 - raiz.N - 1;
		memcpy(&raiz.C[ORDEM / 2 - 2], CDirConcat, sizeof(int));
		memcpy(&raiz.PR[ORDEM / 2 - 2], PRDirConcat, sizeof(int));
		memcpy(&raiz.C[ORDEM / 2 - 1], AuxR.C, sizeof(int) * j);
		memcpy(&raiz.PR[ORDEM / 2 - 1], AuxR.PR, sizeof(int) * j);
		memcpy(&raiz.P[ORDEM / 2 - 1], &AuxR.P, sizeof(int) * (j + 1));
		*CDirConcat = AuxR.C[j];
		*PRDirConcat = AuxR.PR[j];
		memmove(AuxR.C, &AuxR.C[j + 1], sizeof(int) * (ORDEM - 2 - j));
		memmove(AuxR.PR, &AuxR.PR[j + 1], sizeof(int) * (ORDEM - 2 - j));
		memmove(AuxR.P, &AuxR.P[j + 1], sizeof(int) * (ORDEM - 1 - j));
		AuxR.N -= j + 1;
		raiz.N += j + 1;
		escreverPagina(AuxR, arquivoIndices);
		escreverPagina(raiz, arquivoIndices);
		return 1;
	}
	/* Não deu, somos obrigados a concatenar: 3) Tentar concatenar com o irmão da esquerda */
	if(PEsq >= 0) {
		memmove(&raiz.C[AuxL.N + 1], raiz.C, sizeof(int) * raiz.N);
		memmove(&raiz.PR[AuxL.N + 1], raiz.PR, sizeof(int) * raiz.N);
		memmove(&raiz.P[AuxL.N + 1], raiz.P, sizeof(int) * (raiz.N + 1));
		memcpy(&raiz.C[AuxL.N], CEsqConcat, sizeof(int));
		memcpy(&raiz.PR[AuxL.N], PREsqConcat, sizeof(int));
		memcpy(raiz.C, AuxL.C, sizeof(int) * AuxL.N);
		memcpy(raiz.PR, AuxL.PR, sizeof(int) * AuxL.N);
		memcpy(raiz.P, AuxL.P, sizeof(int) * (AuxL.N + 1));
		raiz.N += AuxL.N + 1;
		escreverPagina(raiz, arquivoIndices);
		return 2;
	}
	/* Não deu: 4) Concatenar com o irmão da direita */
	memmove(&AuxR.C[raiz.N + 1], AuxR.C, sizeof(int) * AuxR.N);
	memmove(&AuxR.PR[raiz.N + 1], AuxR.PR, sizeof(int) * AuxR.N);
	memmove(&AuxR.P[raiz.N + 1], AuxR.P, sizeof(int) * (AuxR.N + 1));
	memcpy(&AuxR.C[raiz.N], CDirConcat, sizeof(int));
	memcpy(&AuxR.PR[raiz.N], PRDirConcat, sizeof(int));
	memcpy(AuxR.C, raiz.C, sizeof(int) * raiz.N);
	memcpy(AuxR.PR, raiz.PR, sizeof(int) * raiz.N);
	memcpy(AuxR.P, raiz.P, sizeof(int) * (raiz.N + 1));
	AuxR.N += raiz.N + 1;
	escreverPagina(AuxR, arquivoIndices);
	return 3;
}


