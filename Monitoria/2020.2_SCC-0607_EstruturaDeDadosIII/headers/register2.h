

#ifndef REGISTER2_H_
#define REGISTER2_H_

	typedef struct {
		int RRN;
		char removed;
		int idPessoaQueSegue;
		int idPessoaQueESeguida;
		char grauAmizade[3];
		char dataInicioQueSegue[11];
		char dataFimQueSegue[11];
	} REG2_DATA;

	typedef struct {
		char status;
		int quantidadeSeguidores;
	} REG2_HEADER;

	int R2_readH(REG2_HEADER *dest, FILE *stream);
	int R2_writeH(REG2_HEADER *src, FILE *stream);

	int R2_read(REG2_DATA *dest, FILE *stream);
	int R2_write(REG2_DATA *src, FILE *stream);
	int R2_rewrite(REG2_DATA *src, FILE *stream);
	int R2_print(REG2_DATA *reg);


#endif