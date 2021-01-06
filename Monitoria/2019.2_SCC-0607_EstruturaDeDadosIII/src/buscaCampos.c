#include <stdlib.h>
#include <string.h>
#include <stdio.h>



#include <register.h>
#include <consts.h>



typedef struct CAMPO_PAR_T__ {
	char fieldName[100];
	char fieldValue[100];
	char newFieldName[100];
	char newFieldValue[100];
	struct CAMPO_PAR_T__ *next;
} CAMPO_PAR_T;

typedef struct BUSCA_CAMPO_T__ {
	CAMPO_PAR_T *first;
	char vector;
	int n;
} BUSCA_CAMPO_T;



int BC_searchMarked(CAMPO_PAR_T *campos, int nCampos, int start, REGISTRO_T *reg) {
	int i;
	for(i = start; i < nCampos; i++) {
		if(strcmp(campos[i].fieldName, "RRN") == 0) {
			if(atoi(campos[i].fieldValue) == reg->RRN) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "estadoOrigem") == 0) {
			if(strcmp(campos[i].fieldValue, reg->estadoOrigem) == 0) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "estadoDestino") == 0) {
			if(strcmp(campos[i].fieldValue, reg->estadoDestino) == 0) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "distancia") == 0) {
			if(atoi(campos[i].fieldValue) == reg->distancia) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "cidadeOrigem") == 0) {
			if(strcmp(campos[i].fieldValue, reg->cidadeOrigem) == 0) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "cidadeDestino") == 0) {
			if(strcmp(campos[i].fieldValue, reg->cidadeDestino) == 0) {
				return i;
			}
		} else if(strcmp(campos[i].fieldName, "tempoViagem") == 0) {
			if(strcmp(campos[i].fieldValue, reg->tempoViagem) == 0) {
				return i;
			}
		}
	}
	return -1;
}

BUSCA_CAMPO_T *BC_new() {
	BUSCA_CAMPO_T *aux;

	aux = (BUSCA_CAMPO_T *) malloc(sizeof(BUSCA_CAMPO_T));
	if(aux == NULL) {
		return NULL;
	}

	aux->n = 0;
	aux->vector = 0;
	aux->first = NULL;
	return aux;
}

int BC_asVector(BUSCA_CAMPO_T *BC) {
	CAMPO_PAR_T *newVector, *aux;
	int i;
	if(BC == NULL) {
		return 0;
	}
	if(BC->vector == 1) {
		return 1;
	}
	if(BC->n < 1) {
		BC->first = NULL;
		BC->vector = 1;
		return 1;
	}
	newVector = (CAMPO_PAR_T *) malloc(sizeof(CAMPO_PAR_T) * BC->n);
	if(newVector == NULL) {
		return 0;
	}
	i = BC->n - 1;
	while(BC->first != NULL) {
		aux = BC->first;
		memcpy(&newVector[i--], aux, sizeof(CAMPO_PAR_T));
		BC->first = BC->first->next;
		free(aux);
	}
	BC->first = newVector;
	BC->vector = 1;
	return 1;
}

int BC_asList(BUSCA_CAMPO_T *BC) {
	CAMPO_PAR_T *oldVector, *aux;
	char success;
	int i;
	if(BC == NULL) {
		return 0;
	}
	if(BC->vector != 1) {
		return 1;
	}
	oldVector = BC->first;
	BC->first = NULL;
	success = 1;
	for(i = 0; i < BC->n; i++) {
		aux = (CAMPO_PAR_T *) malloc(sizeof(CAMPO_PAR_T));
		if(aux == NULL) {
			success = 0;
			continue;
		}
		memcpy(aux, &oldVector[i], sizeof(CAMPO_PAR_T));
		aux->next = BC->first;
		BC->first = aux;
	}
	free(oldVector);
	BC->vector = 0;
	return success;
}

int BC_insertUpdate(char *searchField, char *searchValue, char *modifyField, char *modifyValue, BUSCA_CAMPO_T *BC) {
	CAMPO_PAR_T *aux;
	if(searchField == NULL || searchValue == NULL || BC == NULL) {
		return 0;
	}
	if(BC_asList(BC) != 1) {
		return 0;
	}

	aux = (CAMPO_PAR_T *) malloc(sizeof(CAMPO_PAR_T));
	if(aux == NULL) {
		return 0;
	}
	strcpy(aux->fieldName, searchField);
	strcpy(aux->fieldValue, searchValue);
	if(modifyField != NULL) {
		strcpy(aux->newFieldName, modifyField);
		if(modifyValue != NULL) {
			strcpy(aux->newFieldValue, modifyValue);
		} else {
			aux->newFieldValue[0] = '\0';
		}
	} else {
		aux->newFieldName[0] = '\0';
		aux->newFieldValue[0] = '\0';
	}
	aux->next = BC->first;
	BC->first = aux;
	BC->n++;
	return 1;
}

int BC_insertSearch(char *searchField, char *searchValue, BUSCA_CAMPO_T *BC) {
	CAMPO_PAR_T *aux;
	if(searchField == NULL || searchValue == NULL || BC == NULL) {
		return 0;
	}
	return BC_insertUpdate(searchField, searchValue, NULL, NULL, BC);
}

int BC_search(REGISTRO_T *reg, BUSCA_CAMPO_T *BC) {
	if(reg == NULL || BC == NULL || BC_asVector(BC) != 1) {
		return -1;
	}
	if(BC_searchMarked(BC->first, BC->n, 0, reg) >= 0) {
		return 1;
	}
	return 0;
}

int BC_searchUpdate(REGISTRO_T *reg, FILE *stream, BUSCA_CAMPO_T *BC) {
	char modified;
	int pos;
	if(reg == NULL || BC == NULL || BC_asVector(BC) != 1) {
		return -1;
	}
	modified = 0;
	pos = 0;
	while((pos = BC_searchMarked(BC->first, BC->n, pos, reg)) >= 0) {
		if(BC->first[pos].newFieldName[0] == '\0') {
			pos++;
			continue;
		}
		if(strcmp(BC->first[pos].newFieldName, "removido") == 0) { // Remover
			reg->removido = R_TRUE;
		} else if(strcmp(BC->first[pos].newFieldName, "estadoOrigem") == 0) {
			strcpy(reg->estadoOrigem, BC->first[pos].newFieldValue);
		} else if(strcmp(BC->first[pos].newFieldName, "estadoDestino") == 0) {
			strcpy(reg->estadoDestino, BC->first[pos].newFieldValue);
		} else if(strcmp(BC->first[pos].newFieldName, "distancia") == 0) {
			reg->distancia = atoi(BC->first[pos].newFieldValue);
		} else if(strcmp(BC->first[pos].newFieldName, "cidadeOrigem") == 0) {
			strcpy(reg->cidadeOrigem, BC->first[pos].newFieldValue);
		} else if(strcmp(BC->first[pos].newFieldName, "cidadeDestino") == 0) {
			strcpy(reg->cidadeDestino, BC->first[pos].newFieldValue);
		} else if(strcmp(BC->first[pos].newFieldName, "tempoViagem") == 0) {
			strcpy(reg->tempoViagem, BC->first[pos].newFieldValue);
		}
		R_reescreverRegistro(reg, stream);
		modified = 1;
		pos++;
	}
	return modified;
}

void BC_destroy(BUSCA_CAMPO_T *BC) {
	if(BC == NULL) {
		return;
	}
	BC_asVector(BC);
	free(BC->first);
	free(BC);
}
