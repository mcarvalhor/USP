#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <indiceCidades.h>
#include <escreverTela.h>
#include <buscaCampos.h>
#include <graph.h>
#include <register.h>
#include <consts.h>



int f1_csvToBin(char *csvPath, char *binPath) {
	FILE *csvStream, *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	REGISTRO_T aux;

	indice = I_new();
	if(indice == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((csvStream = fopen(csvPath, "r")) == NULL) {
		I_destroy(indice);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((binStream = fopen(binPath, "wb")) == NULL) {
		I_destroy(indice);
		fclose(csvStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	cab.status = R_FALSE;
	cab.numeroArestas = 0;
	cab.numeroVertices = 0;
	cab.dataUltimaCompactacao[0] = '\0';
	R_escreverCabecalho(&cab, binStream);

	aux.removido = R_FALSE;
	fscanf(csvStream, "%*[^\r\n]%*c"); // Ignore first line from CSV
	while(fscanf(csvStream, "%[^,]%*c%[^,]%*c%d%*c%[^,]%*c%[^,]%*c", aux.estadoOrigem, aux.estadoDestino, &aux.distancia, aux.cidadeOrigem, aux.cidadeDestino) == 5) {
		if(fscanf(csvStream, "%[^\r\n]", aux.tempoViagem) != 1) {
			aux.tempoViagem[0] = '\0';
		}
		fscanf(csvStream, "%*c");
		trim(aux.estadoOrigem); // If there's any \n or \r left here, we remove it.
		if(strlen(aux.cidadeOrigem) + strlen(aux.cidadeDestino) + strlen(aux.tempoViagem) > TAM_REGISTRO_DADOS_VARIAVEIS) {
			printf("Ignorando registro %d (%s %s %d %s %s %s) porque os campos variáveis ultrapassam o limite máximo.\n", cab.numeroArestas + 1, aux.estadoOrigem, aux.estadoDestino, aux.distancia, aux.cidadeOrigem, aux.cidadeDestino, aux.tempoViagem);
			continue;
		}
		R_escreverRegistro(&aux, binStream);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
		cab.numeroArestas++;
	}

	cab.status = R_TRUE;
	cab.numeroVertices = I_keyCount(indice);
	R_escreverCabecalho(&cab, binStream);
	I_destroy(indice);
	fclose(csvStream);
	fclose(binStream);
	binarioNaTela1(binPath);
	return 1;
}

int f2_listAll(char *binPath) {
	FILE *binStream;
	CABECALHO_T cab;
	REGISTRO_T aux;
	char found;

	if((binStream = fopen(binPath, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	found = 0;
	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		printf("%d %s %s %d %s %s", aux.RRN, aux.estadoOrigem, aux.estadoDestino, aux.distancia, aux.cidadeOrigem, aux.cidadeDestino);
		if(aux.tempoViagem[0] != '\0') {
			printf(" %s", aux.tempoViagem);
		}
		printf("\n");
		found = 1;
	}

	if(found == 0) {
		printf("Registro inexistente.\n");
	}
	fclose(binStream);
	return 1;
}

int f3_listSearch(char *binPath) {
	char nomeCampo[100], valorCampo[100];
	BUSCA_CAMPO_T *BC;
	FILE *binStream;
	CABECALHO_T cab;
	REGISTRO_T aux;
	char found;

	BC = BC_new();
	if(BC == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}
	
	if((binStream = fopen(binPath, "rb")) == NULL) {
		BC_destroy(BC);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		BC_destroy(BC);
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	while(scanf("%s", nomeCampo) == 1) {
		scan_quote_string(valorCampo);
		BC_insertSearch(nomeCampo, valorCampo, BC);
	}

	found = 0;
	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		if(BC_search(&aux, BC) != 1) {
			continue;
		}
		printf("%d %s %s %d %s %s", aux.RRN, aux.estadoOrigem, aux.estadoDestino, aux.distancia, aux.cidadeOrigem, aux.cidadeDestino);
		if(aux.tempoViagem[0] != '\0') {
			printf(" %s", aux.tempoViagem);
		}
		printf("\n");
		found = 1;
	}

	if(found == 0) {
		printf("Registro inexistente.\n");
	}
	BC_destroy(BC);
	fclose(binStream);
	return 1;
}

int f4_listRRN(int RRN, char *binPath) {
	FILE *binStream;
	CABECALHO_T cab;
	REGISTRO_T aux;
	char found;

	if((binStream = fopen(binPath, "rb")) == NULL) {
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	fseek(binStream, TAM_REGISTRO_CABECALHO + (TAM_REGISTRO_DADOS * RRN), SEEK_SET);
	if(R_lerRegistro(&aux, binStream) == 1 && aux.removido == R_FALSE) {
		printf("%d %s %s %d %s %s", aux.RRN, aux.estadoOrigem, aux.estadoDestino, aux.distancia, aux.cidadeOrigem, aux.cidadeDestino);
		if(aux.tempoViagem[0] != '\0') {
			printf(" %s", aux.tempoViagem);
		}
		printf("\n");
	} else {
		printf("Registro inexistente.\n");
	}

	fclose(binStream);
	return 1;
}

int f5_remove(char *binPath) {
	char nomeCampo[100], valorCampo[100];
	BUSCA_CAMPO_T *BC;
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	REGISTRO_T aux;

	indice = I_new();
	BC = BC_new();
	if(indice == NULL || BC == NULL || (binStream = fopen(binPath, "rb+")) == NULL) {
		I_destroy(indice);
		BC_destroy(BC);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		BC_destroy(BC);
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	while(scanf("%s", nomeCampo) == 1) {
		scan_quote_string(valorCampo);
		BC_insertUpdate(nomeCampo, valorCampo, "removido", NULL, BC); // Uses "update" to remove the register.
	}

	cab.status = R_FALSE;
	R_escreverCabecalho(&cab, binStream);

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
		if(BC_searchUpdate(&aux, binStream, BC) != 1) {
			continue;
		}
		I_remove(aux.cidadeOrigem, indice);
		I_remove(aux.cidadeDestino, indice);
		cab.numeroArestas--;
	}

	cab.status = R_TRUE;
	cab.numeroVertices = I_keyCount(indice);
	R_escreverCabecalho(&cab, binStream);
	I_destroy(indice);
	BC_destroy(BC);
	fclose(binStream);
	binarioNaTela1(binPath);
	return 1;
}

int f6_insert(char *binPath) {
	char nomeCampo[100], valorCampo[100];
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	REGISTRO_T aux;

	indice = I_new();
	if(indice == NULL || (binStream = fopen(binPath, "rb+")) == NULL) {
		I_destroy(indice);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	cab.status = R_FALSE;
	R_escreverCabecalho(&cab, binStream);

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
	}

	//fseek(binStream, 0, SEEK_END); // Go to end of file.

	aux.removido = R_FALSE;
	while(scanf("%s %s %d", aux.estadoOrigem, aux.estadoDestino, &aux.distancia) == 3) {
		scan_quote_string(aux.cidadeOrigem);
		scan_quote_string(aux.cidadeDestino);
		scan_quote_string(aux.tempoViagem);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
		R_escreverRegistro(&aux, binStream);
		cab.numeroArestas++;
	}

	cab.status = R_TRUE;
	cab.numeroVertices = I_keyCount(indice);
	R_escreverCabecalho(&cab, binStream);
	I_destroy(indice);
	fclose(binStream);
	binarioNaTela1(binPath);
	return 1;
}

int f7_update(char *binPath) {
	char buscaCampoRRN[100], nomeCampo[100], valorCampo[100];
	BUSCA_CAMPO_T *BC;
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	REGISTRO_T aux;

	indice = I_new();
	BC = BC_new();
	if(indice == NULL || BC == NULL || (binStream = fopen(binPath, "rb+")) == NULL) {
		I_destroy(indice);
		BC_destroy(BC);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		BC_destroy(BC);
		fclose(binStream);
		printf("Falha no processamento do arquivo.\n");
		return 0;
	}

	while(scanf("%s %s", buscaCampoRRN, nomeCampo) == 2) {
		scan_quote_string(valorCampo);
		BC_insertUpdate("RRN", buscaCampoRRN, nomeCampo, valorCampo, BC); // Uses "update" to remove the register.
	}

	cab.status = R_FALSE;
	R_escreverCabecalho(&cab, binStream);

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		if(BC_searchUpdate(&aux, binStream, BC) != 1) {
			I_insert(aux.cidadeOrigem, indice);
			I_insert(aux.cidadeDestino, indice);
			continue;
		}
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
	}

	cab.status = R_TRUE;
	cab.numeroVertices = I_keyCount(indice);
	R_escreverCabecalho(&cab, binStream);
	I_destroy(indice);
	BC_destroy(BC);
	fclose(binStream);
	binarioNaTela1(binPath);
	return 1;
}

int f8_compactar(char *oldBinPath, char *newBinPath) {
	FILE *oldBinStream, *newBinStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	REGISTRO_T aux;

	indice = I_new();
	if(indice == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((oldBinStream = fopen(oldBinPath, "rb")) == NULL) {
		I_destroy(indice);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, oldBinStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		fclose(oldBinStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((newBinStream = fopen(newBinPath, "wb")) == NULL) {
		I_destroy(indice);
		fclose(oldBinStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	cab.status = R_FALSE;
	cab.numeroArestas = 0;
	cab.numeroVertices = 0;
	strcpy(cab.dataUltimaCompactacao, R_DATA_COMPACTACAO);
	R_escreverCabecalho(&cab, newBinStream);

	while(R_lerRegistro(&aux, oldBinStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		R_escreverRegistro(&aux, newBinStream);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
		cab.numeroArestas++;
	}

	cab.status = R_TRUE;
	cab.numeroVertices = I_keyCount(indice);
	R_escreverCabecalho(&cab, newBinStream);
	I_destroy(indice);
	fclose(oldBinStream);
	fclose(newBinStream);
	binarioNaTela1(newBinPath);
	return 1;
}

void f9_printAresta(int vertice, int distancia, char *tempoViagem, INDEX_T *ind) {
	if(tempoViagem == NULL || tempoViagem[0] == '\0') {
		printf(" %s %d", I_getKeyFor(vertice, ind), distancia);
	} else {
		printf(" %s %d %s", I_getKeyFor(vertice, ind), distancia, tempoViagem);
	}
}

int f9_gerarGrafo(char *binPath) {
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	GRAPH_T *grafo;
	REGISTRO_T aux;
	long i;

	indice = I_new();
	if(indice == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((binStream = fopen(binPath, "rb")) == NULL) {
		I_destroy(indice);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		fclose(binStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
	}
	
	grafo = G_new(cab.numeroVertices);
	fseek(binStream, TAM_REGISTRO_CABECALHO, SEEK_SET);
	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeOrigem, indice), I_getIndexFor(aux.cidadeDestino, indice), aux.distancia, aux.tempoViagem, grafo);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeDestino, indice), I_getIndexFor(aux.cidadeOrigem, indice), aux.distancia, aux.tempoViagem, grafo); // Não-direcionado
	}

	for(i = 0; i < cab.numeroVertices; i++) {
		printf("%s", I_getKeyFor(i, indice));
		G_printEdgesFor(i, (void *(*)(int, int, char *, void *)) &f9_printAresta, indice, grafo);
		printf("\n");
	}

	I_destroy(indice);
	G_destroy(grafo);
	fclose(binStream);
	return 1;
}

int f10_caminhoMaisCurto(char *binPath) {
	char city[INDEX_KEY_MAX_LEN];
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	GRAPH_T *grafo;
	REGISTRO_T aux;
	long i, vertex;
	int *pre, *dis;

	scanf("%s", city);
	scan_quote_string(city);

	indice = I_new();
	if(indice == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((binStream = fopen(binPath, "rb")) == NULL) {
		I_destroy(indice);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		fclose(binStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
	}
	
	grafo = G_new(cab.numeroVertices);
	fseek(binStream, TAM_REGISTRO_CABECALHO, SEEK_SET);
	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeOrigem, indice), I_getIndexFor(aux.cidadeDestino, indice), aux.distancia, aux.tempoViagem, grafo);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeDestino, indice), I_getIndexFor(aux.cidadeOrigem, indice), aux.distancia, aux.tempoViagem, grafo); // Não-direcionado
	}

	I_fillRestOfKey(city, indice);
	vertex = I_getIndexFor(city, indice);
	if(vertex < 0) {
		printf("Cidade inexistente.\n");
		I_destroy(indice);
		G_destroy(grafo);
		fclose(binStream);
		return 0;
	}

	G_dijkstra(vertex, &pre, &dis, NULL, grafo);
	for(i = 0; i < cab.numeroVertices; i++) {
		if(pre[i] < 0 || i == vertex) {
			continue;
		}
		printf("%s %s %d %s\n", city, I_getKeyFor(i, indice), dis[i], I_getKeyFor(pre[i], indice));
	}

	I_destroy(indice);
	G_destroy(grafo);
	fclose(binStream);
	return 1;
}

int f11_arvoreGeradoraMinima(char *binPath) {
	char city[INDEX_KEY_MAX_LEN];
	long i, j, vertex;
	FILE *binStream;
	INDEX_T *indice;
	CABECALHO_T cab;
	GRAPH_T *grafo;
	REGISTRO_T aux;
	int *pre, *dis;

	scanf("%s", city);
	scan_quote_string(city);

	indice = I_new();
	if(indice == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	if((binStream = fopen(binPath, "rb")) == NULL) {
		I_destroy(indice);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	R_lerCabecalho(&cab, binStream);
	if(cab.status != R_TRUE) {
		I_destroy(indice);
		fclose(binStream);
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		I_insert(aux.cidadeOrigem, indice);
		I_insert(aux.cidadeDestino, indice);
	}
	
	grafo = G_new(cab.numeroVertices);
	fseek(binStream, TAM_REGISTRO_CABECALHO, SEEK_SET);
	while(R_lerRegistro(&aux, binStream) == 1) {
		if(aux.removido != R_FALSE) {
			continue;
		}
		strcat(aux.cidadeOrigem, " ");
		strcat(aux.cidadeDestino, " ");
		strcat(aux.cidadeOrigem, aux.estadoOrigem);
		strcat(aux.cidadeDestino, aux.estadoDestino);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeOrigem, indice), I_getIndexFor(aux.cidadeDestino, indice), aux.distancia, aux.tempoViagem, grafo);
		G_addEdgeInfo(I_getIndexFor(aux.cidadeDestino, indice), I_getIndexFor(aux.cidadeOrigem, indice), aux.distancia, aux.tempoViagem, grafo); // Não-direcionado
	}

	I_fillRestOfKey(city, indice);
	vertex = I_getIndexFor(city, indice);
	if(vertex < 0) {
		printf("Cidade inexistente.\n");
		I_destroy(indice);
		G_destroy(grafo);
		fclose(binStream);
		return 0;
	}

	G_prim(vertex, &pre, &dis, NULL, grafo);
	for(i = 0; i < cab.numeroVertices; i++) {
		printf("%s", I_getKeyFor(i, indice));
		for(j = 0; j < cab.numeroVertices; j++) {
			if(pre[j] == i) {
				if(G_getInfo(i, j, grafo) == NULL || G_getInfo(i, j, grafo)[0] == '\0') {
					printf(" %s %d", I_getKeyFor(j, indice), dis[j]);
				} else {
					printf(" %s %d %s", I_getKeyFor(j, indice), dis[j], G_getInfo(i, j, grafo));
				}
			}
		}
		printf("\n");
	}

	I_destroy(indice);
	G_destroy(grafo);
	fclose(binStream);
	return 1;
}

int fx_barraR(char binary, char *filePath) {
	char *buffer, fileOpening[10];
	size_t i, streamLen;
	FILE *stream;

	if(binary == R_FALSE || binary == 0) {
		strcpy(fileOpening, "r");
	} else {
		strcpy(fileOpening, "rb");
	}

	if((stream = fopen(filePath, fileOpening)) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	fseek(stream, 0, SEEK_END);
	streamLen = ftell(stream);
	buffer = (char *) malloc(sizeof(char) * streamLen);
	fseek(stream, 0, SEEK_SET);
	fread(buffer, sizeof(char) * streamLen, 1, stream);
	fclose(stream);

	for(i = 0; i < streamLen; i++) {
		if(buffer[i] == '\r') {
			printf("Existe barra R ('\\r') nesse arquivo! Verifique a posição 0x%zx do arquivo.\n", i);
			return 1;
		}
	}
	printf("Não existe nenhum barra R ('\\r') nesse arquivo.\n");
	return 1;
}

int fx_invalidate(char *filePath) {
	char *buffer, aux;
	FILE *stream;

	if((stream = fopen(filePath, "rb+")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	aux = R_FALSE;
	fseek(stream, 0, SEEK_SET);
	fwrite(&aux, sizeof(char), 1, stream);
	fclose(stream);
	printf("Arquivo invalidado com sucesso.\n");
	return 1;
}

int fx_validate(char *filePath) {
	char *buffer, aux;
	FILE *stream;

	if((stream = fopen(filePath, "rb+")) == NULL) {
		printf("Falha no carregamento do arquivo.\n");
		return 0;
	}

	aux = R_TRUE;
	fseek(stream, 0, SEEK_SET);
	fwrite(&aux, sizeof(char), 1, stream);
	fclose(stream);
	printf("Arquivo revalidado com sucesso.\n");
	return 1;
}


