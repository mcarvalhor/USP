#include <stdlib.h>
#include <stdio.h>
#include <processman.h>



/*
 *	~  Gerenc. de Processos ~
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



int main(int argc, char **argv){
	PROC_LIST *Processes;
	if(getchar()=='f'){
		Processes=NewSession(proc_fifo);
	}else{
		fseek(stdin,-1,SEEK_CUR); // Voltar para escanear os números da forma correta.
		Processes=NewSession(proc_round_robin);
	}
	IncludeProcessesFrom(stdin,Processes); // Ler processos considerando que tenham a mesma prioridade (FIFO).
	RunProcesses(Processes); // Rodar como se fosse uma fila.
	PrintResults(stdout,Processes);
	DestroySession(Processes);
	return EXIT_SUCCESS;
}


