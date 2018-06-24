#include <stdio.h>
#include <stdlib.h>
#include <funcionalidades.h>



/*
 *	~       Trabalho Prático: Parte 2       ~
 *
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



/* == Definição do nome do arquivo binário *.DAT */

#define NOME_ARQUIVO_BINARIO "registros.dat"
#define NOME_ARQUIVO_INDICES "indice-arvoreb.dat"
#define NOME_ARQUIVO_CONTADOR "buffer-info.text"



/* == Enumeração de argumentos do programa == */

enum {
	NOME_PROG,
	OPERACAO,
	ARG1,
	ARG2,
	ARG3,
	ARG4,
	ARG5,
	ARG6,
	ARG7
};



/* == MAIN == */

int main(int argc, char **argv) {
	int codigoOperacao;

	if(argc < 2) { /* Verificar se o usuário está passando os argumentos necessários */
		printf("Uso:\n\t%s (CODIGO_OPERACAO) [...POSSIVEIS_ARGUMENTOS]\n", argv[NOME_PROG]);
		return EXIT_FAILURE;
	}

	codigoOperacao = atoi(argv[OPERACAO]);

	switch (codigoOperacao) {

		case 10:
		case 1: /* Transferir todos os registros do *.CSV para o *.DAT */
			if (argc != 3) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (NOME_ARQUIVO_CSV)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			transferirCSV(argv[ARG1], NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 2: /* Recuperar todos os registros do binário */
			if (argc != 2) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			recuperarTudo(NOME_ARQUIVO_BINARIO);
		break;

		case 3: /* Buscar registros por um campo específico */
			if (argc != 4) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (NOME_CAMPO) (VALOR_BUSCADO)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			recuperarPorCampo(argv[ARG1], argv[ARG2], NOME_ARQUIVO_BINARIO);
		break;

		case 4: /* Recuperar registro por RRN */
			if (argc != 3) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (RRN)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			recuperarPorRRN(atoi(argv[ARG1]), NOME_ARQUIVO_BINARIO);
		break;

		case 5: /* Remover registro por RRN */
			if (argc != 3) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (RRN)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			removerRegistro(atoi(argv[ARG1]), NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 11:
		case 6: /* Inserir registro */
			if (argc != 8) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (CAMPO1=codINEP) (CAMPO2=dataAtiv) (CAMPO3=uf) (CAMPO4=nomeEscola) (CAMPO5=municipio) (CAMPO6=prestadora)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			inserirRegistro(&argv[ARG1], NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 7: /* Atualizar registro */
			if (argc != 9) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (RRN) (CAMPO1=codINEP) (CAMPO2=dataAtiv) (CAMPO3=uf) (CAMPO4=nomeEscola) (CAMPO5=municipio) (CAMPO6=prestadora)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			atualizarRegistro(atoi(argv[ARG1]), &argv[ARG2], NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 8: /* Desfragmentar arquivo binário */
			if (argc != 2) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			desfragmentarBinario(NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 9: /* Imprimir pilha de registros removidos */
			if (argc != 2) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			imprimirPilha(NOME_ARQUIVO_BINARIO);
		break;

		case 12: /* Buscar registro através da árvore-B */
			if (argc != 3) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (CHAVE=codINEP)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			recuperarPorChave(atoi(argv[ARG1]), NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 13: /* Remover registro através da árvore-B */
			if (argc != 3) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (CHAVE=codINEP)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			removerRegistroChave(atoi(argv[ARG1]), NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		case 14: /* Atualizar registro através da árvore-B */
			if (argc != 9) { /* Verificar se o usuário está passando os argumentos corretamente */
				printf("Uso para a operação %d:\n\t%s %d (CHAVE=codINEP) (CAMPO1=codINEP) (CAMPO2=dataAtiv) (CAMPO3=uf) (CAMPO4=nomeEscola) (CAMPO5=municipio) (CAMPO6=prestadora)\n", codigoOperacao, argv[NOME_PROG], codigoOperacao);
				return EXIT_FAILURE;
			}
			atualizarRegistroChave(atoi(argv[ARG1]), &argv[ARG2], NOME_ARQUIVO_BINARIO, NOME_ARQUIVO_INDICES, NOME_ARQUIVO_CONTADOR);
		break;

		default: /* Código de operação inválido */
			printf("Código de operação deve estar entre 1 e 14.\nUso:\n\t%s (CODIGO_OPERACAO) [...POSSIVEIS_ARGUMENTOS]\n", argv[NOME_PROG]);
			return EXIT_FAILURE;
		break;
	}

	return EXIT_SUCCESS;
}


