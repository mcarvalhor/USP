
#ifndef CONSTS_H_
#define CONSTS_H_

	#define TAM_REGISTRO_DADOS 85
	#define TAM_REGISTRO_DADOS_VARIAVEIS TAM_REGISTRO_DADOS - 2 - 2 - 4 - 3
	#define TAM_REGISTRO_CABECALHO 19
	#define TAM_LINE_BUFFER 1024*10
	#define TAM_STR_BUFFER 100

	#define R_TRUE '1'
	#define R_FALSE '0'

	#define R_REMOVIDO '*'
	#define R_DELIMITADOR '|'
	#define R_LIXO '#'
	#define R_NIL '\0'

	#define R_DATA_COMPACTACAO "01/11/2019"

	#define INDEX_HASHTABLE_LEN 1024*8
	#define INDEX_KEY_MAX_LEN 100


#endif