#include <stdio.h>
#include <stdlib.h>

#include <funcionalidades.h>
#include <consts.h>
#include <escreverTela.h>
#include <vector_builder.h>

int icmp(int *a, int *b) {
	return *b - *a;
}

int ircmp(int *a, int *b) {
	return *a - *b;
}

int main(int argc, char **argv) {
	char aux1[5000], aux2[5000], aux3[5000];
	int Op, aux4;

	scanf("%d", &Op);

	switch(Op) {
		case 1: // CSV -> Bin
			scanf("%s", aux1);
			if(f1_csvBuild(BINARY_DISK_FILE, aux1)) {
				binarioNaTela2(BINARY_DISK_FILE);
				//fprintf(stdout, "%s", BINARY_DISK_FILE);
			}
		break;
		case 2: // imprimir tudo tela
			scanf("%s", aux1);
			f2_list(aux1);
		break;
		case 3: // busca por determinado campo
			scanf("%s %s %[^\r\n]", aux1, aux2, aux3);
			f3_search(aux1, aux2, aux3);
		break;
		case 4: // remoção
			scanf("%s %d", aux1, &aux4);
			if(f4_removeRegs(aux1, aux4, stdin)) {
				binarioNaTela2(aux1);
			}
		break;
		case 5: // inserção
			scanf("%s %d", aux1, &aux4);
			if(f5_addRegs(aux1, aux4, stdin)) {
				binarioNaTela2(aux1);
			}
		break;
		case 6: // atualização
			scanf("%s %d", aux1, &aux4);
			if(f6_updateRegs(aux1, aux4, stdin)) {
				binarioNaTela2(aux1);
			}
		break;
		case 7: // ordenar
			scanf("%s %s", aux1, aux2);
			if(f7_sort(aux1, aux2)) {
				binarioNaTela2(aux2);
			}
		break;
		case 8: // merge
			scanf("%s %s %s", aux1, aux2, aux3);
			if(f8_merge(aux1, aux2, aux3)) {
				binarioNaTela2(aux3);
			}
		break;
		case 9: // match
			scanf("%s %s %s", aux1, aux2, aux3);
			if(f9_match(aux1, aux2, aux3)) {
				binarioNaTela2(aux3);
			}
		break;
		case 10: // criar index
			scanf("%s %s", aux1, aux2);
			if(f10_index(aux1, aux2)) {
				binarioNaTela2(aux2);
			}
		break;
		case 11: // busca por nomeServidor no index
			scanf("%s %s %*s %[^\r\n]", aux1, aux2, aux3);
			f11_searchKey(aux1, aux2, aux3);
		break;
		case 12: // remove por nomeServidor no index
			scanf("%s %s %d", aux1, aux2, &aux4);
			if(f12_removeRegsKey(aux1, aux2, aux4, stdin) == 1) {
				binarioNaTela2(aux2);
			}
		break;
		case 13: // adiciona regitros, usando o index
			scanf("%s %s %d", aux1, aux2, &aux4);
			if(f13_addRegsKey(aux1, aux2, aux4, stdin) == 1) {
				binarioNaTela2(aux2);
			}
		break;
		case 14: // busca por nomeServidor no index, gerando estatisticas
			scanf("%s %s %*s %[^\r\n]", aux1, aux2, aux3);
			f14_searchKeyStats(aux1, aux2, aux3);
		break;
		case 15: // criar btree
			scanf("%s %s", aux1, aux2);
			if(f15_indexB(aux1, aux2)) {
				binarioNaTela2(aux2);
			}
		break;
		case 16: // busca por idServidor na btree
			scanf("%s %s %*s %[^\r\n]", aux1, aux2, aux3);
			f16_searchKeyB(aux1, aux2, aux3);
		break;

		/* Funcionalidades p/ criação de casos de teste */
		case 98: // Invalidar arquivo binário (status = '0')
			scanf("%s", aux1);
			fx_invalidate(aux1);
		break;
		case 99: // Imprimir arquivo binário (binarioNaTela)
			scanf("%s", aux1);
			binarioNaTela2(aux1);
		break;
		case 100: // Imprimir lista de itens removidos de arquivo binário de dados
			scanf("%s", aux1);
			fx_listRemoved(aux1);
		break;

		default:
			printf("Funcionalidade inválida.\n");
			return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
