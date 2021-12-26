#include <stdlib.h>
#include <stdio.h>

#include <funcionalidades.h>
#include <consts.h>

int main(int argc, char **argv) {
	char arg1[TAM_STR_BUFFER], arg2[TAM_STR_BUFFER], arg3[TAM_STR_BUFFER], arg4[TAM_STR_BUFFER];
	int Op;

	// Scan operation
	scanf("%d", &Op);

	switch(Op) {
		case 1:
			scanf("%s %s", arg1, arg2);
			f1_csvToBin(arg1, arg2);
		break;
		case 2:
			scanf("%s", arg1);
			f2_listAll(arg1);
		break;
		case 3:
			scanf("%s", arg1);
			f3_listSearch(arg1);
		break;
		case 4:
			scanf("%s %s", arg1, arg2);
			f4_listRRN(atoi(arg2), arg1);
		break;
		case 5:
			scanf("%s %*d", arg1);
			f5_remove(arg1);
		break;
		case 6:
			scanf("%s %*d", arg1);
			f6_insert(arg1);
		break;
		case 7:
			scanf("%s %*d", arg1);
			f7_update(arg1);
		break;
		case 8:
			scanf("%s %s", arg1, arg2);
			f8_compactar(arg1, arg2);
		break;

		case 9:
			scanf("%s", arg1);
			f9_gerarGrafo(arg1);
		break;
		case 10:
			scanf("%s", arg1);
			f10_caminhoMaisCurto(arg1);
		break;
		case 11:
			scanf("%s", arg1);
			f11_arvoreGeradoraMinima(arg1);
		break;

		case 100: // tem barra R ('\r') no arquivo?
			scanf("%s", arg1);
			fx_barraR(1, arg1);
		break;
		case 101: // invalidate binary file.
			scanf("%s", arg1);
			fx_invalidate(arg1);
		break;
		case 102: // revalidate binary file.
			scanf("%s", arg1);
			fx_validate(arg1);
		break;

		default:
			printf("Operação inválida!\n");
	}

	return EXIT_SUCCESS;
}
