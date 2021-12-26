

#ifndef H_REGISTER
#define H_REGISTER

	typedef struct {
		long byteOffset;
		char removed;
		int size;
		long next;
		int idServidor;
		double salarioServidor;
		char telefoneServidor[14];
		char nomeServidor[5000];
		char cargoServidor[5000];
	} REG_DATA;

	typedef struct {
		char status;
		long first;
		char desCampos[5][40];
	} REG_HEADER;

	int R_readH(REG_HEADER *dest, FILE *stream);
	int R_writeH(REG_HEADER *src, FILE *stream);

	int R_read(REG_DATA *dest, FILE *stream);
	int R_write(REG_DATA *src, FILE *stream, long *lastOffset);
	int R_rewriteBack(REG_DATA *src, FILE *stream);
	int R_calculateSize(REG_DATA *r);


#endif