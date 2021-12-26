#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "consts.h"
#include "register.h"



int R_readH(REG_HEADER *dest, FILE *stream) {
	REG_HEADER aux;
	char reg[64];
	int i, n;

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	if(fread(reg, sizeof(char), 64, stream) != 64) {
		return 0;
	}

	aux.status = reg[0];
	memcpy(&aux.quantidadePessoas, &reg[1], sizeof(int));
	memcpy(dest, &aux, sizeof(REG_HEADER));

	return 1;
}

int R_writeH(REG_HEADER *src, FILE *stream) {
	char reg[64], str[41];
	int i, n;

	memset(reg, LIXO, sizeof(char) * 64);

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	memcpy(reg, &src->status, sizeof(char));
	memcpy(&reg[1], &src->quantidadePessoas, sizeof(int));

	if(fwrite(reg, sizeof(char), 64, stream) != 64) {
		return 0;
	}

	return 1;
}

int R_read(REG_DATA *dest, FILE *stream) {
	char reg[64], *R;
	REG_DATA aux;
	long offset;
	int i, n;

	memset(reg, LIXO, sizeof(char) * 64);

	aux.RRN = (ftell(stream) - 64) / 64;
	if(aux.RRN < 0 || fread(reg, sizeof(char), 64, stream) != 64) {
		return 0;
	}

	R = reg;
	memcpy(&aux.removed, R, sizeof(char));
	R += sizeof(char);
	memcpy(&aux.idPessoa, R, sizeof(int));
	R += sizeof(int);
	memcpy(&aux.nomePessoa, R, sizeof(char) * 40);
	R += sizeof(char) * 40;
	memcpy(&aux.idadePessoa, R, sizeof(int));
	R += sizeof(int);
	memcpy(&aux.twitterPessoa, R, sizeof(char) * 15);

	memcpy(dest, &aux, sizeof(REG_DATA));
	return 1;
}

int R_write(REG_DATA *src, FILE *stream) {
	char reg[64], *R;
	REG_DATA aux;

	memset(reg, LIXO, sizeof(char) * 64);
	src->removed = R_NAO_REMOVIDO;
	src->RRN = (ftell(stream) - 64) / 64;

	if(src->RRN < 0) {
		return 0;
	}

	/* Truncamento */
	if(strlen(src->nomePessoa) >= 40) {
		src->nomePessoa[39] = '\0';
	}
	memset(src->nomePessoa + strlen(src->nomePessoa) + 1, LIXO, sizeof(char) * (100 - strlen(src->nomePessoa) - 1));
	if(strlen(src->twitterPessoa) >= 15) {
		src->twitterPessoa[14] = '\0';
	}
	memset(src->twitterPessoa + strlen(src->twitterPessoa) + 1, LIXO, sizeof(char) * (100 - strlen(src->twitterPessoa) - 1));

	R = reg;
	memcpy(R, &src->removed, sizeof(char));
	R += sizeof(char);
	memcpy(R, &src->idPessoa, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->nomePessoa, sizeof(char) * 40);
	R += sizeof(char) * 40;
	memcpy(R, &src->idadePessoa, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->twitterPessoa, sizeof(char) * 15);

	if(fwrite(reg, sizeof(char), 64, stream) != 64) {
		return 0;
	}

	return 1;
}

int R_rewrite(REG_DATA *src, FILE *stream) {
	char reg[64], *R;
	REG_DATA aux;

	memset(reg, LIXO, sizeof(char) * 64);

	if(src->RRN < 0 || fseek(stream, 64 + 64 * src->RRN, SEEK_SET) != 0) {
		return 0;
	}

	if(src->removed == R_REMOVIDO) {
		reg[0] = R_REMOVIDO;
		if(fwrite(reg, sizeof(char), 64, stream) != 64) {
			return 0;
		}
		return 1;
	}

	/* Truncamento */
	if(strlen(src->nomePessoa) >= 40) {
		src->nomePessoa[39] = '\0';
	}
	memset(src->nomePessoa + strlen(src->nomePessoa) + 1, LIXO, sizeof(char) * (100 - strlen(src->nomePessoa) - 1));
	if(strlen(src->twitterPessoa) >= 15) {
		src->twitterPessoa[14] = '\0';
	}
	memset(src->twitterPessoa + strlen(src->twitterPessoa) + 1, LIXO, sizeof(char) * (100 - strlen(src->twitterPessoa) - 1));

	R = reg;
	*R = R_NAO_REMOVIDO;
	R += sizeof(char);
	memcpy(R, &src->idPessoa, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->nomePessoa, sizeof(char) * 40);
	R += sizeof(char) * 40;
	memcpy(R, &src->idadePessoa, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->twitterPessoa, sizeof(char) * 15);

	if(fwrite(reg, sizeof(char), 64, stream) != 64) {
		return 0;
	}

	return 1;
}

int R_print(REG_DATA *reg) {
	if(reg->removed == R_REMOVIDO) {
		return 0;
	}
	printf("Dados da pessoa de código %d\n", reg->idPessoa);
	if(reg->nomePessoa[0] == '\0') {
		printf("Nome: %s\n", "-");
	} else {
		printf("Nome: %s\n", reg->nomePessoa);
	}
	if(reg->idadePessoa < 0) {
		printf("Idade: -\n");
	} else {
		printf("Idade: %d anos\n", reg->idadePessoa);
	}
	printf("Twitter: %s\n\n", reg->twitterPessoa);
	return 1;
}

