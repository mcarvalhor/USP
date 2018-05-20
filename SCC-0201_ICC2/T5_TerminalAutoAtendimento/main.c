#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ATM.h"



/*
 *	~  Relatório Fluxo ATM  ~
 *
 *	Matheus Carvalho Raimundo
 *	mcarvalhor@usp.br
 *	 _______ _______ _______
 *	|   |   |               \
 *	|   |   |      \    |___|
 *	|   |    \      |   |
 *	|_______ _______|___|
 *
*/



// Número de "caixas automáticos".
#define ATM_COUNT 4
// Tamanho fixo da HashTable.
#define MAX_SIZE 2048
// Lucro por cada operação (3 reais).
#define PROFIT 3



int main(int argc, char **argv){
	int i, Aux, ATM, Limit, Bank1, Bank2, ErrorATMs[ATM_COUNT][4], ErrorTotal[4];
	double ProfitATMs[ATM_COUNT], ProfitTotal = 0, Money;
	HASHTABLE *Operations, *ATMs[ATM_COUNT], *Total;
	char Command;

	// Zerar variáveis e inicializar HashTables
	Operations=New_HashTable(sizeof(OPERATION), NULL, MAX_SIZE);
	Total=New_HashTable(sizeof(BANK), NULL, MAX_SIZE);
	memset(ErrorTotal, 0, sizeof(ErrorTotal));
	memset(ErrorATMs, 0, sizeof(ErrorATMs));
	memset(ProfitATMs, 0, sizeof(ProfitATMs));
	for(i=0;i<ATM_COUNT;i++)
		ATMs[i]=New_HashTable(sizeof(BANK), NULL, MAX_SIZE);

	while( (Aux=ReadOperation(&Command, &ATM, &Limit, &Bank1, &Bank2, &Money))>0 ){ // Ler operação.
		if(Command < 0)	break;
		switch(Command){
			case 'S': // Saque.
				if(Aux == 3){ // Erro.
					ErrorATMs[ATM][0]++;
					ErrorTotal[0]++;
					continue;
				}
				Withdraw(Money, Bank1, ATMs[ATM]);
				Withdraw(Money, Bank1, Total);
				AddAudit(ATM, audit_withdraw, Bank1, -1, Money, Operations);
			break;
			case 'D': // Depósito.
				if(Aux == 3){ // Erro.
					ErrorATMs[ATM][1]++;
					ErrorTotal[1]++;
					continue;
				}
				Deposit(Money, Bank1, ATMs[ATM]);
				Deposit(Money, Bank1, Total);
				AddAudit(ATM, audit_deposit, Bank1, -1, Money, Operations);
			break;
			case 'U': // Transferência bancos diferentes.
				if(Aux == 3){ // Erro.
					ErrorATMs[ATM][2]++;
					ErrorTotal[2]++;
					continue;
				}
				TransferFrom(Money, Bank1, ATMs[ATM]);
				TransferTo(Money, Bank2, ATMs[ATM]);
				TransferFrom(Money, Bank1, Total);
				TransferTo(Money, Bank2, Total);
				AddAudit(ATM, audit_transfer, Bank1, Bank2, Money, Operations);
			break;
			case 'T': // Transferência mesmo banco.
				if(Aux == 3){ // Erro.
					ErrorATMs[ATM][2]++;
					ErrorTotal[2]++;
					continue;
				}
				AddAudit(ATM, audit_transfer, Bank1, Bank1, Money, Operations);
			break;
			default: // Consulta.
				if(Aux == 3){ // Erro.
					ErrorATMs[ATM][3]++;
					ErrorTotal[3]++;
					continue;
				}
			break;
		}
		ProfitATMs[ATM]+=PROFIT;
		ProfitTotal+=PROFIT;
		if(Aux == 2)	break; // Fim da entrada.
	}

	for(i=0;i<ATM_COUNT;i++){ // Imprimir informações processadas de cada terminal.
		printf("===TERMINAL %d===\n", i+1);
		PrintATM(ATMs[i]);
		Destroy_HashTable(ATMs[i]);
		printf("Lucro obtido: %.2lf\n", ProfitATMs[i]);
		if(ErrorATMs[i][0] > 0)	printf("Erros de saque: %d\n", ErrorATMs[i][0]);
		if(ErrorATMs[i][1] > 0)	printf("Erros de deposito: %d\n", ErrorATMs[i][1]);
		if(ErrorATMs[i][3] > 0)	printf("Erros de consulta: %d\n", ErrorATMs[i][3]);
		if(ErrorATMs[i][2] > 0)	printf("Erros de transferencia: %d\n", ErrorATMs[i][2]);
		if(ErrorATMs[i][0]+ErrorATMs[i][1]+ErrorATMs[i][2]+ErrorATMs[i][3] > 0)	printf("Total de erros: %d\n", ErrorATMs[i][0]+ErrorATMs[i][1]+ErrorATMs[i][2]+ErrorATMs[i][3]);
	}
	// Imprimir total de informações processadas.
	printf("===TOTAL===\n");
	PrintATM(Total);
	Destroy_HashTable(Total);
	printf("Lucro obtido: %.2lf\n", ProfitTotal);
	if(ErrorTotal[0] > 0)	printf("Erros de saque: %d\n", ErrorTotal[0]);
	if(ErrorTotal[1] > 0)	printf("Erros de deposito: %d\n", ErrorTotal[1]);
	if(ErrorTotal[3] > 0)	printf("Erros de consulta: %d\n", ErrorTotal[3]);
	if(ErrorTotal[2] > 0)	printf("Erros de transferencia: %d\n", ErrorTotal[2]);
	if(ErrorTotal[0]+ErrorTotal[1]+ErrorTotal[2]+ErrorTotal[3] > 0)	printf("Total de erros: %d\n", ErrorTotal[0]+ErrorTotal[1]+ErrorTotal[2]+ErrorTotal[3]);

	if(Command < 0){ // Auditoria.
		printf("===AUDITORIA===\n");
		do {
			Command*=-1;
			switch(Command){
				case 'S': // Saque.
					printf("===SAQUE TERMINAL %d===\n", ATM+1);
					PrintAudit(ATM, audit_withdraw, Limit, Operations);
				break;
				case 'D': // Depósito.
					printf("===DEPOSITO TERMINAL %d===\n", ATM+1);
					PrintAudit(ATM, audit_deposit, Limit, Operations);
				break;
				case 'U': // Transferência mesmo banco.
				case 'T': // Transferência bancos diferentes.
					printf("===TRANSFERENCIA TERMINAL %d===\n", ATM+1);
					PrintAudit(ATM, audit_transfer, Limit, Operations);
				break;
				default:
				break;
			}
			if(Aux == 2)	break; // Fim da entrada.
		} while( (Aux=ReadOperation(&Command, &ATM, &Limit, &Bank1, &Bank2, &Money))>0 ); // Ler mais pedidos de auditoria, se tiver.
	}

	Destroy_HashTable(Operations);

	return EXIT_SUCCESS;
}


