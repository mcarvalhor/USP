#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <hashtable.h>
#include "ATM.h"



/*
 *	~          ATM          ~
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



int ReadOperation(char *Command, int *ATM, int *Limit, int *Bank1, int *Bank2, double *Money) {
	/*
	* Esta função lê da entrada padrão (stdin) uma operação no formato especificado no trabalho.
	* Ela preenche todos os dados relevantes das operações nos ponteiros para variável declarados nos argumentos.
	*
	* Ela retorna 3 em caso de erros, 2 caso seja a última operação da Stream, 1 caso esteja tudo ok, ou -1 em caso de erros.
	*/
	if(Command==NULL || ATM==NULL || Limit==NULL || Bank1==NULL || Bank2==NULL || Money==NULL)
		return -1;
	double Aux1, Aux2;
	char Aux3[1000], Aux4;
	scanf("%d %c", ATM, Command);
	if(*Command=='S' || *Command=='D' || *Command=='C' || *Command=='T'){
		scanf("%d", Limit);
		*Command*=-1; // "Auditoria".
	}else{
		fseek(stdin, -1, SEEK_CUR);
		scanf("%d %c", Bank1, Command);
		switch(*Command){
			case 'S': // Saque.
				scanf("%lf", Money);
			break;
			case 'D': // Depósito.
				scanf("%lf", Money);
			break;
			case 'T': // Transferência
				fgets(Aux3, sizeof(Aux3)/sizeof(char), stdin);
				if(sscanf(Aux3, "%lf %lf", &Aux1, &Aux2)==2){
					sscanf(Aux3, "%d %lf", Bank2, Money);
					*Command='U'; // Transferência bancos diferentes.
				}else{
					sscanf(Aux3, "%lf", Money);
				}
			break;
			default: // Por último, consulta.
			break;
		}
	}
	*ATM-=1;
	while( (Aux4=getchar())!=EOF && isspace(Aux4))	{ }
	if(Aux4=='E'){ // "ERRO".
		fgets(Aux3, sizeof(Aux3)/sizeof(char), stdin);
		return 3;
	}else if(Aux4==EOF){ // Fim da entrada.
		return 2;
	}
	fseek(stdin, -1, SEEK_CUR);
	return 1; // Ok.
}

int Withdraw(double Money, int Bank, HASHTABLE *HT){
	/*
	* Esta função faz um saque de 'Money' do banco 'Bank' através do terminal 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	BANK Aux;
	BANK *Aux2=&Aux;
	if(Search_From_HashTable(Bank, &Aux2, HT)<1){ // Primeira inserção.
		memset(&Aux, 0, sizeof(BANK));
		Aux.Bank=Bank;
		Aux.WithdrawnMoney=Money;
		Insert_Into_HashTable(Bank, &Aux, HT);
	}else{ // Apenas mudar valor.
		Aux2[0].WithdrawnMoney+=Money;
	}
	return 1;
}

int Deposit(double Money, int Bank, HASHTABLE *HT){
	/*
	* Esta função faz um depósito de 'Money' do banco 'Bank' através do terminal 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	BANK Aux;
	BANK *Aux2=&Aux;
	if(Search_From_HashTable(Bank, &Aux2, HT)<1){ // Primeira inserção.
		memset(&Aux, 0, sizeof(BANK));
		Aux.Bank=Bank;
		Aux.DepositedMoney=Money;
		Insert_Into_HashTable(Bank, &Aux, HT);
	}else{ // Apenas mudar valor.
		Aux2[0].DepositedMoney+=Money;
	}
	return 1;
}

int TransferFrom(double Money, int Bank, HASHTABLE *HT){
	/*
	* Esta função faz uma transferência de 'Money' do banco 'Bank' através do terminal 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	BANK Aux;
	BANK *Aux2=&Aux;
	if(Search_From_HashTable(Bank, &Aux2, HT)<1){ // Primeira inserção.
		memset(&Aux, 0, sizeof(BANK));
		Aux.Bank=Bank;
		Aux.TransfersFrom=Money;
		Insert_Into_HashTable(Bank, &Aux, HT);
	}else{ // Apenas mudar valor.
		Aux2[0].TransfersFrom+=Money;
	}
	return 1;
}

int TransferTo(double Money, int Bank, HASHTABLE *HT){
	/*
	* Esta função faz uma transferência de 'Money' para o banco 'Bank' através do terminal 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	BANK Aux;
	BANK *Aux2=&Aux;
	if(Search_From_HashTable(Bank, &Aux2, HT)<1){ // Primeira inserção.
		memset(&Aux, 0, sizeof(BANK));
		Aux.Bank=Bank;
		Aux.TransfersTo=Money;
		Insert_Into_HashTable(Bank, &Aux, HT);
	}else{ // Apenas mudar valor.
		Aux2[0].TransfersTo+=Money;
	}
	return 1;
}

int AddAudit(int ATM, char Command, int Bank1, int Bank2, double Money, HASHTABLE *HT){
	/*
	* Esta função adiciona uma operação realizada a HashTable 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	OPERATION Aux;
	Aux.Bank1=Bank1;
	Aux.Bank2=Bank2;
	Aux.Money=Money;
	Insert_Into_HashTable(Command|(ATM<<2), &Aux, HT);
	return 1;
}

void PrintATM_Line(void *Bank){
	/*
	* Esta é uma função auxiliar. Ela é usada para imprimir uma linha de informações de um banco de um determinado terminal.
	* Ela é auxiliar de "PrintATM".
	*/
	BANK *Aux=Bank;
	printf("Banco %d: Moeda +%.2lf -%.2lf Transferencia +%.2lf -%.2lf\n", Aux->Bank, Aux->DepositedMoney, Aux->WithdrawnMoney, Aux->TransfersTo, Aux->TransfersFrom);
}

int PrintATM(HASHTABLE *HT){
	/*
	* Esta função imprime informações processadas no terminal 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	Traverse_HashTable(PrintATM_Line, HT);
	return 1;
}

int PrintAudit(int ATM, char Command, unsigned long Limit, HASHTABLE *HT){
	/*
	* Esta função imprime informações de uma audição solicitada diretamente da HashTable 'HT'.
	*
	* Ela retorna 1 em caso de sucesso, ou 0 em caso de erros.
	*/
	if(HT==NULL)	return 0;
	OPERATION *Aux;
	unsigned long i, n=Search_From_HashTable(Command|(ATM<<2), &Aux, HT);
	printf("Mostrando primeiros %ld resultados\n", Limit);
	if(n==0){
		printf("Sem resultados\n");
	}else{
		for(i=0;i<n && Limit>0;i++, Limit--){
			if(Aux[i].Bank2>0)	printf("%ld- Banco origem %d Banco destino %d %.2lf\n", i+1, Aux[i].Bank1, Aux[i].Bank2, Aux[i].Money);
			else	printf("%ld- Banco %d %.2lf\n", i+1, Aux[i].Bank1, Aux[i].Money);
		}
	}
	return 1;
}


