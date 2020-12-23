#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <consts.h>
#include <register.h>
#include <buffer.h>



int R_readH(REG_HEADER *dest, FILE *stream) {
	char reg[PAGE_SIZE], *R;
	REG_HEADER aux;
	int i, n;

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	if(fread(reg, sizeof(char), PAGE_SIZE, stream) != PAGE_SIZE) {
		return 0;
	}

	R = reg;
	aux.status = *R;
	R++;
	memcpy(&aux.first, R, sizeof(long));
	R += sizeof(long) + sizeof(char);
	memcpy(aux.desCampos[0], R, sizeof(char) * 40);
	R += sizeof(char) * 41;
	memcpy(aux.desCampos[1], R, sizeof(char) * 40);
	R += sizeof(char) * 41;
	memcpy(aux.desCampos[2], R, sizeof(char) * 40);
	R += sizeof(char) * 41;
	memcpy(aux.desCampos[3], R, sizeof(char) * 40);
	R += sizeof(char) * 41;
	memcpy(aux.desCampos[4], R, sizeof(char) * 40);
	R += sizeof(char) * 41;

	memcpy(dest, &aux, sizeof(REG_HEADER));

	return 1;
}

int R_writeH(REG_HEADER *src, FILE *stream) {
	char reg[PAGE_SIZE], *R, str[41];
	int i, n;

	memset(reg, LIXO, sizeof(char) * PAGE_SIZE);

	if(fseek(stream, 0, SEEK_SET) != 0) {
		return 0;
	}

	R = reg;
	memcpy(R, &src->status, sizeof(char));
	R++;
	memcpy(R, &src->first, sizeof(long));
	R += sizeof(long);
	memset(str, LIXO, sizeof(char) * 41);
	str[0] = 'i';
	strcpy(str + 1, src->desCampos[0]);
	memcpy(R, str, sizeof(char) * 41);
	R += sizeof(char) * 41;
	memset(str, LIXO, sizeof(char) * 41);
	str[0] = 's';
	strcpy(str + 1, src->desCampos[1]);
	memcpy(R, str, sizeof(char) * 41);
	R += sizeof(char) * 41;
	memset(str, LIXO, sizeof(char) * 41);
	str[0] = 't';
	strcpy(str + 1, src->desCampos[2]);
	memcpy(R, str, sizeof(char) * 41);
	R += sizeof(char) * 41;
	memset(str, LIXO, sizeof(char) * 41);
	str[0] = 'n';
	strcpy(str + 1, src->desCampos[3]);
	memcpy(R, str, sizeof(char) * 41);
	R += sizeof(char) * 41;
	memset(str, LIXO, sizeof(char) * 41);
	str[0] = 'c';
	strcpy(str + 1, src->desCampos[4]);
	memcpy(R, str, sizeof(char) * 41);
	R += sizeof(char) * 41;

	if(fwrite(reg, sizeof(char), PAGE_SIZE, stream) != PAGE_SIZE) {
		return 0;
	}

	return 1;
}

int R_read(REG_DATA *dest, FILE *stream) {
	char reg[PAGE_SIZE], *R;
	REG_DATA aux;
	long offset;
	int i, n;

	memset(reg, LIXO, sizeof(char) * PAGE_SIZE);

	while((reg[0] = getc(stream)) == LIXO && reg[0] != EOF); // Se ele tiver lendo lixo (como no finalzinho de uma página por exemplo)

	if(reg[0] == EOF) { // Fim do arquivo
		return 0;
	}

	offset = ftell(stream) - sizeof(char);
	if(fread(&reg[1], sizeof(int), 1, stream) != 1) { // Ler tamanho registro
		return -1;
	}
	R = reg;

	aux.removed = *R;
	aux.byteOffset = offset;
	R++;
	memcpy(&aux.size, R, sizeof(int));
	R += sizeof(int);

	if(fread(R, sizeof(char), aux.size, stream) != aux.size) {
		return -1;
	}

	memcpy(&aux.next, R, sizeof(long));
	R += sizeof(long);
	memcpy(&aux.idServidor, R, sizeof(int));
	R += sizeof(int);
	memcpy(&aux.salarioServidor, R, sizeof(double));
	R += sizeof(double);
	memcpy(&aux.telefoneServidor, R, sizeof(char) * 14);
	R += sizeof(char) * 14;
	memcpy(&n, R, sizeof(int));
	R += sizeof(int);
	if(*R == 'n') { // Nome não é nulo
		R++;
		strcpy(aux.nomeServidor, R);
		R += n - 1;
		memcpy(&n, R, sizeof(int));
		R += sizeof(int);
		if(*R == 'c') { // Cargo não é nulo
			R++;
			strcpy(aux.cargoServidor, R);
		} else { // Os dois são nulos
			aux.cargoServidor[0] = '\0';
		}
	} else if(*R == 'c') { // Nome é nulo, mas cargo não
		aux.nomeServidor[0] = '\0';
		R++;
		strcpy(aux.cargoServidor, R);
	} else { // Os dois são nulos
		aux.nomeServidor[0] = '\0';
		aux.cargoServidor[0] = '\0';
	}

	memcpy(dest, &aux, sizeof(REG_DATA));
	return 1;
}

int R_write(REG_DATA *src, FILE *stream, long *lastSize) {
	char reg[PAGE_SIZE], *R;
	int i, n, sn, filledPage;
	unsigned long pn;
	REG_DATA aux;

	memset(reg, LIXO, sizeof(char) * PAGE_SIZE);
	n = sizeof(long) + sizeof(int) + sizeof(double) + sizeof(char) * 14;

	if(strcmp(src->nomeServidor, "") != 0) {
		n += strlen(src->nomeServidor) + sizeof(int) + 2;
	}

	if(strcmp(src->cargoServidor, "") != 0) {
		n += strlen(src->cargoServidor) + sizeof(int) + 2;
	}

	pn = B_offset(stream);
	if(pn < 0) {
		return -1;
	}

	filledPage = 1;
	pn = PAGE_SIZE - pn;
	if(pn < n + sizeof(int) + sizeof(char)) { // Registro não cabe nessa página, preencher com lixo e ir pra próxima pra poder escrever
		if(fwrite(reg, sizeof(char), pn, stream) != pn) {
			return -1;
		}
		if(*lastSize >= 0) {
			fseek(stream, -1 * ((*lastSize) + sizeof(int) + pn), SEEK_CUR);
			sn = pn + (*lastSize);
			fwrite(&sn, sizeof(int), 1, stream);
			fseek(stream, 0, SEEK_END);
			filledPage = 2;
		}
	}

	src->removed = R_NAO_REMOVIDO;
	src->size = n;
	src->next = -1;

	R = reg;
	memcpy(R, &src->removed, sizeof(char));
	R += sizeof(char);
	memcpy(R, &src->size, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->next, sizeof(long));
	R += sizeof(long);
	memcpy(R, &src->idServidor, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->salarioServidor, sizeof(double));
	R += sizeof(double);
	if(src->telefoneServidor[0] == '\0') {
		memset(src->telefoneServidor + sizeof(char), LIXO, sizeof(char) * 13);
	}
	memcpy(R, &src->telefoneServidor, sizeof(char) * 14);
	R += sizeof(char) * 14;

	if(strcmp(src->nomeServidor, "") != 0) {
		sn = strlen(src->nomeServidor) + 2;
		memcpy(R, &sn, sizeof(int));
		R += sizeof(int);
		*R = 'n';
		R++;
		strcpy(R, src->nomeServidor);
		R += sn - 1;
		*(R - 1) = '\0';
	}

	if(strcmp(src->cargoServidor, "") != 0) {
		sn = strlen(src->cargoServidor) + 2;
		memcpy(R, &sn, sizeof(int));
		R += sizeof(int);
		*R = 'c';
		R++;
		strcpy(R, src->cargoServidor);
		R += sn - 1;
		*(R - 1) = '\0';
	}

	*lastSize = n;
	src->byteOffset = ftell(stream);
	if(fwrite(reg, sizeof(char), n + sizeof(int) + sizeof(char), stream) != n + sizeof(int) + sizeof(char)) {
		return -1;
	}

	return filledPage;
}

int R_rewriteBack(REG_DATA *src, FILE *stream) {
	char reg[PAGE_SIZE], *R;
	unsigned long pn;
	REG_DATA aux;
	int i, /*n,*/ lastn, sn;


	memset(reg, LIXO, sizeof(char) * PAGE_SIZE);

	lastn = src->size + sizeof(int) + sizeof(char);
	if(fseek(stream, src->byteOffset, SEEK_SET) != 0) {
		return -1;
	}

	R = reg;
	memcpy(R, &src->removed, sizeof(char));
	R += sizeof(char);
	memcpy(R, &src->size, sizeof(int));
	R += sizeof(int);
	memcpy(R, &src->next, sizeof(long));
	R += sizeof(long);

	if(src->removed == R_NAO_REMOVIDO) {
		memcpy(R, &src->idServidor, sizeof(int));
		R += sizeof(int);
		memcpy(R, &src->salarioServidor, sizeof(double));
		R += sizeof(double);
		if(src->telefoneServidor[0] == '\0') {
			memset(src->telefoneServidor + sizeof(char), LIXO, sizeof(char) * 13);
		}
		memcpy(R, &src->telefoneServidor, sizeof(char) * 14);
		R += sizeof(char) * 14;
		
		if(strcmp(src->nomeServidor, "") != 0) {
			sn = strlen(src->nomeServidor) + 2;
			memcpy(R, &sn, sizeof(int));
			R += sizeof(int);
			*R = 'n';
			R++;
			strcpy(R, src->nomeServidor);
			R += sn - 1;
			*(R - 1) = '\0';
		}

		if(strcmp(src->cargoServidor, "") != 0) {
			sn = strlen(src->cargoServidor) + 2;
			memcpy(R, &sn, sizeof(int));
			R += sizeof(int);
			*R = 'c';
			R++;
			strcpy(R, src->cargoServidor);
			R += sn - 1;
			*(R - 1) = '\0';
		}
	}
	
	if(fwrite(reg, sizeof(char), lastn, stream) != lastn) {
		return -1;
	}

	return 1;
}

int R_calculateSize(REG_DATA *r) {
	int n;
	n = sizeof(long) + sizeof(int) + sizeof(double) + sizeof(char) * 14;

	if(strcmp(r->nomeServidor, "") != 0) {
		n += strlen(r->nomeServidor) + sizeof(int) + 2;
	}

	if(strcmp(r->cargoServidor, "") != 0) {
		n += strlen(r->cargoServidor) + sizeof(int) + 2;
	}
	return n;
}

int R_writePad(FILE *stream) { // OBSOLETA: faz o padding de LIXO na última página do arquivo binário.
	char reg[PAGE_SIZE];
	unsigned long pn;

	memset(reg, '@', sizeof(char) * PAGE_SIZE);

	pn = B_offset(stream);
	if(pn < 0) {
		return -1;
	}
	
	pn = PAGE_SIZE - pn;
	if(pn > 0) {
		if(fwrite(reg, sizeof(char), pn, stream) != pn) {
			return -1;
		}
		return 1;
	}

	return 1;
}


