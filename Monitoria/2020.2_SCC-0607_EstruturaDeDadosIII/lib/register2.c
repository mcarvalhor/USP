#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "consts.h"
#include "register2.h"



int R2_readH(REG2_HEADER *dest, FILE *stream) {
	REG2_HEADER aux;
	char reg[32];
	int i, n;

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	if(fread(reg, sizeof(char), 32, stream) != 32) {
		return 0;
	}

	aux.status = reg[0];
	memcpy(&aux.quantidadeSeguidores, &reg[1], sizeof(int));
	memcpy(dest, &aux, sizeof(REG2_HEADER));

	return 1;
}

int R2_writeH(REG2_HEADER *src, FILE *stream) {
	char reg[32];
	int i, n;

	memset(reg, LIXO, sizeof(char) * 32);

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	memcpy(reg, &src->status, sizeof(char));
	memcpy(&reg[1], &src->quantidadeSeguidores, sizeof(int));

	if(fwrite(reg, sizeof(char), 32, stream) != 32) {
		return 0;
	}

	return 1;
}

int R2_read(REG2_DATA *dest, FILE *stream) {
	char reg[32], *R;
	REG2_DATA aux;
	long offset;
	int i, n;

	memset(reg, LIXO, sizeof(char) * 32);

	aux.RRN = (ftell(stream) - 32) / 32;
	if(aux.RRN < 0 || fread(reg, sizeof(char), 32, stream) != 32) {
		return 0;
	}

	R = reg;
	memcpy(&aux.removed, R, sizeof(char));
	R += sizeof(char);
	memcpy(&aux.idPessoaQueSegue, R, sizeof(int));
	R += sizeof(int);
	memcpy(&aux.idPessoaQueESeguida, R, sizeof(int));
	R += sizeof(int);
	memcpy(&aux.grauAmizade, R, sizeof(char) * 3);
	R += sizeof(char) * 3;
	memcpy(&aux.dataInicioQueSegue, R, sizeof(char) * 10);
	R += sizeof(char) * 10;
	memcpy(&aux.dataFimQueSegue, R, sizeof(char) * 10);
	R += sizeof(char) * 10;

	aux.dataInicioQueSegue[10] = '\0';
	aux.dataFimQueSegue[10] = '\0';

	memcpy(dest, &aux, sizeof(REG2_DATA));
	return 1;
}

int R2_write(REG2_DATA *src, FILE *stream) {
	char reg[32], *R;
	REG2_DATA aux;

	memset(reg, LIXO, sizeof(char) * 32);
	src->removed = R_NAO_REMOVIDO;
	src->RRN = (ftell(stream) - 32) / 32;

	if(src->RRN < 0) {
		return 0;
	}

	src->grauAmizade[1] = '\0';
	src->grauAmizade[2] = LIXO;

	R = reg;
	memcpy(R, &src->removed, sizeof(char));
	R += sizeof(char);
	memcpy(R, &src->idPessoaQueSegue, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->idPessoaQueESeguida, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->grauAmizade, sizeof(char) * 3);
	R += sizeof(char) * 3;
	memcpy(R, &src->dataInicioQueSegue, sizeof(char) * 10);
	R += sizeof(char) * 10;
	memcpy(R, &src->dataFimQueSegue, sizeof(char) * 10);
	R += sizeof(char) * 10;

	if(fwrite(reg, sizeof(char), 32, stream) != 32) {
		return 0;
	}

	return 1;
}

int R2_rewrite(REG2_DATA *src, FILE *stream) {
	char reg[32], *R;
	REG2_DATA aux;

	memset(reg, LIXO, sizeof(char) * 32);

	if(src->RRN < 0 || fseek(stream, 32 + 32 * src->RRN, SEEK_SET) != 0) {
		return 0;
	}

	if(src->removed == R_REMOVIDO) {
		reg[0] = R_REMOVIDO;
		if(fwrite(reg, sizeof(char), 32, stream) != 32) {
			return 0;
		}
		return 1;
	}

	src->grauAmizade[1] = '\0';
	src->grauAmizade[2] = LIXO;

	R = reg;
	*R = R_NAO_REMOVIDO;
	R += sizeof(char);
	memcpy(R, &src->idPessoaQueSegue, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->idPessoaQueESeguida, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->grauAmizade, sizeof(char) * 3);
	R += sizeof(char) * 3;
	memcpy(R, &src->dataInicioQueSegue, sizeof(char) * 10);
	R += sizeof(char) * 10;
	memcpy(R, &src->dataFimQueSegue, sizeof(char) * 10);
	R += sizeof(char) * 10;

	if(fwrite(reg, sizeof(char), 32, stream) != 32) {
		return 0;
	}

	return 1;
}

int R2_print(REG2_DATA *reg) {
	if(reg->removed == R_REMOVIDO) {
		return 0;
	}
	printf("Segue a pessoa de código: %d\n", reg->idPessoaQueESeguida);
	printf("Justificativa para seguir: ");
	switch(reg->grauAmizade[0]) {
		case '0':
			printf("segue porque é uma celebridade\n");
		break;
		case '1':
			printf("segue porque é amiga de minha amiga\n");
		break;
		case '2':
			printf("segue porque é minha amiga\n");
		break;
		default:
			printf("-\n");
	}
	printf("Começou a seguir em: %s\n", reg->dataInicioQueSegue);
	printf("Parou de seguir em: %s\n\n", reg->dataFimQueSegue);
	return 1;
}

