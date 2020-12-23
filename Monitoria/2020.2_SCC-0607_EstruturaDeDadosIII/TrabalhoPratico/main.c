#include <stdlib.h>
#include <stdio.h>

#include "funcionalidades.h"
#include "fornecido.h"

int main(int argc, char **argv) {
	char auxs1[100], auxs2[100], auxs3[100], auxs4[100];
	int funcionalidade;

	scanf("%d", &funcionalidade);
	switch(funcionalidade) {
		case 1:
			scanf("%s %s %s", auxs1, auxs2, auxs3);
			f1_readCsv(auxs1, auxs2, auxs3);
		break;
		case 2:
			scanf("%s", auxs1);
			f2_listAll(auxs1);
		break;
		case 3:
			scanf("%s %s", auxs1, auxs2);
			f3_search(auxs1, auxs2);
		break;
		case 4:
			scanf("%s %s", auxs1, auxs2);
			f4_insert(auxs1, auxs2);
		break;
		case 5:
			scanf("%s %s", auxs1, auxs2);
			f5_update(auxs1, auxs2);
		break;
		case 6:
			scanf("%s %s", auxs1, auxs2);
			f6_readCsv(auxs1, auxs2);
		break;
		case 7:
			scanf("%s %s", auxs1, auxs2);
			f7_sort(auxs1, auxs2);
		break;
		case 8:
			scanf("%s %s %*s %s %s", auxs1, auxs2, auxs3, auxs4);
			f8_listMatch(auxs1, auxs2, auxs4, auxs3);
		break;
		case 9:
			scanf("%s %s %s", auxs1, auxs2, auxs3);
			f9_graph(auxs1, auxs2, auxs3);
		break;
		case 10:
			scanf("%s %s %s", auxs1, auxs2, auxs3);
			f10_graphInverted(auxs1, auxs2, auxs3);
		break;
		case 11:
			scanf("%s %s %s", auxs1, auxs2, auxs3);
			scan_quote_string(auxs4);
			f11_bfs(auxs1, auxs2, auxs3, auxs4);
		break;
		case 12:
			scanf("%s %s %s", auxs1, auxs2, auxs3);
			scan_quote_string(auxs4);
			f12_dfsCycle(auxs1, auxs2, auxs3, auxs4);
		break;
		case 100:
			scanf("%s", auxs1);
			fx_invalidadeStatus(auxs1);
		break;
		case 101:
			scanf("%s", auxs1);
			fx_compressFile2(auxs1);
		break;
	}

	return EXIT_SUCCESS;
}