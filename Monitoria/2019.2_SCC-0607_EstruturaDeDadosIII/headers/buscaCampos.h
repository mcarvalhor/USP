
#ifndef H_BUSCACAMPOS_
#define H_BUSCACAMPOS_
#include <stdio.h>
#include <register.h>

	typedef struct BUSCA_CAMPO_T__ BUSCA_CAMPO_T;

	BUSCA_CAMPO_T *BC_new();

	int BC_insertUpdate(char *searchField, char *searchValue, char *modifyField, char *modifyValue, BUSCA_CAMPO_T *BC);
	int BC_insertSearch(char *searchField, char *searchValue, BUSCA_CAMPO_T *BC);

	int BC_search(REGISTRO_T *reg, BUSCA_CAMPO_T *BC);
	int BC_searchUpdate(REGISTRO_T *reg, FILE *stream, BUSCA_CAMPO_T *BC);

	void BC_destroy(BUSCA_CAMPO_T *BC);

#endif