#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <register.h>
#include <consts.h>


int R_lerCabecalho(CABECALHO_T *dest, FILE *stream) {
	char buff[TAM_REGISTRO_CABECALHO + 1], *ptr;
	long pos, newPos;

	pos = ftell(stream);
	memset(dest, R_NIL, sizeof(CABECALHO_T));
	memset(buff, R_NIL, TAM_REGISTRO_CABECALHO + 1);
	if(pos < 0 || fseek(stream, 0, SEEK_SET) != 0 || fread(&dest->status, sizeof(char), 1, stream) != 1) {
		dest->status = R_FALSE; // Forces FALSE state.
		return 0;
	}

	if(dest->status != R_TRUE) { // Header was read successfully, but file is corrupt.
		dest->status = R_FALSE;
		return 1;
	}

	buff[0] = dest->status;
	ptr = buff + 1;
	if(fread(ptr, TAM_REGISTRO_CABECALHO - 1, 1, stream) != 1) {
		return 0;
	}
	memcpy(&dest->numeroVertices, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(&dest->numeroArestas, ptr, sizeof(int));
	ptr += sizeof(int);
	memcpy(dest->dataUltimaCompactacao, ptr, sizeof(char) * 10);
	ptr += 10;

	if((newPos = ftell(stream)) < 0) {
		return 0;
	}
	if(newPos < pos) {
		return (fseek(stream, pos, SEEK_SET) == 0) ? 1:0;
	}
	return 1;
}

int R_escreverCabecalho(CABECALHO_T *src, FILE *stream) {
	char buff[TAM_REGISTRO_CABECALHO + 1], *ptr;
	long pos, newPos;
	char rewrite;

	pos = ftell(stream);
	if(pos < 0 || fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	rewrite = R_TRUE;
	if(fread(buff, TAM_REGISTRO_CABECALHO, 1, stream) != 1) {
		rewrite = R_FALSE;
	}

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	memset(buff, R_LIXO, TAM_REGISTRO_CABECALHO);
	buff[TAM_REGISTRO_CABECALHO] = R_NIL;

	ptr = buff;
	memset(ptr, ((src->status == R_TRUE) ? R_TRUE:R_FALSE), sizeof(char));
	ptr += 1;
	if(src->status == R_TRUE) {
		memcpy(ptr, &src->numeroVertices, sizeof(int));
		ptr += sizeof(int);
		memcpy(ptr, &src->numeroArestas, sizeof(int));
		ptr += sizeof(int);
		memcpy(ptr, src->dataUltimaCompactacao, strlen(src->dataUltimaCompactacao));
		ptr += strlen(src->dataUltimaCompactacao);
	}

	if(rewrite == R_FALSE) {
		if(fwrite(buff, TAM_REGISTRO_CABECALHO, 1, stream) != 1) {
			return 0;
		}
		return 1;
	}

	if(fwrite(buff, ptr - buff, 1, stream) != 1) {
		return 0;
	}
	if((newPos = ftell(stream)) < 0) {
		return 0;
	}
	if(newPos < pos) {
		return (fseek(stream, pos, SEEK_SET) == 0) ? 1:0;
	}
	return 1;
}




int R_lerRegistro(REGISTRO_T *dest, FILE *stream) {
	char buff[TAM_REGISTRO_DADOS + 1], *ptr;
	long pos;

	pos = ftell(stream);
	memset(buff, R_NIL, TAM_REGISTRO_DADOS + 1);
	if(pos < 0 || fread(buff, TAM_REGISTRO_DADOS, 1, stream) != 1) {
		return 0;
	}

	memset(dest, R_NIL, sizeof(REGISTRO_T));
	dest->RRN = (pos - TAM_REGISTRO_CABECALHO) / TAM_REGISTRO_DADOS;
	if(buff[0] == R_REMOVIDO) {
		dest->removido = R_TRUE;
		return 1;
	}

	dest->removido = R_FALSE;
	ptr = buff;
	memcpy(dest->estadoOrigem, ptr, sizeof(char) * 2);
	ptr += 2;
	memcpy(dest->estadoDestino, ptr, sizeof(char) * 2);
	ptr += 2;
	memcpy(&dest->distancia, ptr, sizeof(int));
	ptr += sizeof(int);

	// Strings written in file do not end in '\0', but '|' instead
	sscanf(ptr, "%[^|]", dest->cidadeOrigem);
	ptr += strlen(dest->cidadeOrigem) + 1;
	sscanf(ptr, "%[^|]", dest->cidadeDestino);
	ptr += strlen(dest->cidadeDestino) + 1;
	sscanf(ptr, "%[^|]", dest->tempoViagem);
	return 1;
}

int R_escreverRegistro(REGISTRO_T *src, FILE *stream) {
	char buff[TAM_REGISTRO_DADOS + 1], *ptr;
	long pos;

	pos = ftell(stream);
	if(pos < 0) {
		return 0;
	}

	memset(buff, R_LIXO, TAM_REGISTRO_DADOS);
	buff[TAM_REGISTRO_DADOS] = R_NIL;
	ptr = buff;
	if(src->removido == R_TRUE) {
		memset(ptr, R_REMOVIDO, 1);
		ptr += 1;
	} else {
		memcpy(ptr, src->estadoOrigem, sizeof(char) * 2);
		ptr += 2;
		memcpy(ptr, src->estadoDestino, sizeof(char) * 2);
		ptr += 2;
		memcpy(ptr, &src->distancia, sizeof(int));
		ptr += sizeof(int);
		memcpy(ptr, src->cidadeOrigem, sizeof(char) * strlen(src->cidadeOrigem));
		ptr += strlen(src->cidadeOrigem);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
		memcpy(ptr, src->cidadeDestino, sizeof(char) * strlen(src->cidadeDestino));
		ptr += strlen(src->cidadeDestino);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
		memcpy(ptr, src->tempoViagem, sizeof(char) * strlen(src->tempoViagem));
		ptr += strlen(src->tempoViagem);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
	}

	src->RRN = (pos - TAM_REGISTRO_CABECALHO) / TAM_REGISTRO_DADOS;
	if(fwrite(buff, TAM_REGISTRO_DADOS, 1, stream) != 1) {
		return 0;
	}
	return 1;
}

int R_reescreverRegistro(REGISTRO_T *src, FILE *stream) {
	char buff[TAM_REGISTRO_DADOS + 1], *ptr;
	long len, pos;

	if(src->RRN < 0) {
		return 0;
	}
	pos = ftell(stream);
	len = TAM_REGISTRO_CABECALHO + (src->RRN * TAM_REGISTRO_DADOS);
	if(pos < 0 || fseek(stream, len, SEEK_SET) != 0) {
		return 0;
	}
	if(fread(buff, TAM_REGISTRO_DADOS, 1, stream) != 1) { // Verificar se realmente já tem um registro escrito aqui.
		return 0;
	}
	if(fseek(stream, len, SEEK_SET) != 0) {
		return 0;
	}

	memset(buff, R_LIXO, TAM_REGISTRO_DADOS);
	buff[TAM_REGISTRO_DADOS] = R_NIL;
	ptr = buff;
	if(src->removido == R_TRUE) {
		memset(ptr, R_REMOVIDO, 1);
		ptr += 1;
	} else {
		memcpy(ptr, src->estadoOrigem, sizeof(char) * 2);
		ptr += 2;
		memcpy(ptr, src->estadoDestino, sizeof(char) * 2);
		ptr += 2;
		memcpy(ptr, &src->distancia, sizeof(int));
		ptr += sizeof(int);
		memcpy(ptr, src->cidadeOrigem, sizeof(char) * strlen(src->cidadeOrigem));
		ptr += strlen(src->cidadeOrigem);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
		memcpy(ptr, src->cidadeDestino, sizeof(char) * strlen(src->cidadeDestino));
		ptr += strlen(src->cidadeDestino);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
		memcpy(ptr, src->tempoViagem, sizeof(char) * strlen(src->tempoViagem));
		ptr += strlen(src->tempoViagem);
		memset(ptr, R_DELIMITADOR, sizeof(char));
		ptr += 1;
	}

	if(fwrite(buff, ptr - buff, 1, stream) != 1) {
		return 0;
	}
	if(fseek(stream, pos, SEEK_SET) != 0) {
		return 0;
	}
	return 1;
}

