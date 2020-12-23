

#ifndef REGISTER_H_
#define REGISTER_H_

	typedef struct {
		int RRN;
		char removed;
		int idPessoa;
		char nomePessoa[100];
		int idadePessoa;
		char twitterPessoa[100];
	} REG_DATA;

	typedef struct {
		char status;
		int quantidadePessoas;
	} REG_HEADER;

	int R_readH(REG_HEADER *dest, FILE *stream);
	int R_writeH(REG_HEADER *src, FILE *stream);

	int R_read(REG_DATA *dest, FILE *stream);
	int R_write(REG_DATA *src, FILE *stream);
	int R_rewrite(REG_DATA *src, FILE *stream);
	int R_print(REG_DATA *reg);


#endif