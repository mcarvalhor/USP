#include <stdlib.h>
#include <stdio.h>



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

/*
* Para gerenciar os processos, utilizei o conceito da Heap.
*
* Muitos sistemas baseados em Linux utilizam da Heap para gerenciar processos,
* 	mas eles utilizam uma Min-Heap, pois quanto menor o valor, maior a prioridade.
* Como nesse trabalho é o contrário, quanto maior o valor, maior a prioridade,
* 	vou utilizar uma Max-Heap.
*
* Funcionará de acordo com o método Round Robin, onde há 4 prioridades, e os processos
* 	são executados com base na prioridade, ou no PID quando no mesmo nível.
*
* Além disso, implantei a prioridade de tempo real, e o método FIFO.
*/



enum __proc_man_t { proc_fifo = 0, proc_round_robin }; // Tipo de gerenciamento dos processos.

struct __process_t { // Estrutura que representa um único processo.
	int PID,Priority,TickStart,TickCount;
	int RunningPoint; // Flag que ajuda no gerenciamento da Heap.
};

struct __processlist_t { // Estrutura que representa todos os processos.
	int QueueN, RunningN, FinishedN;
	enum __proc_man_t ProcManType; // Tipo (FIFO ou Round Robin).
	struct __process_t *Queued; // Lista de processos aguardando para serem executados.
	struct __process_t *Running; // Lista de processos em execução.
	struct __process_t *Finished; // Lista de processos encerrados.
};



struct __processlist_t *NewSession(enum __proc_man_t org_type){
	/*
	* Esta função inicia uma nova sessão de processos. "org_type" indica se será FIFO ou Round Robin.
	*
	* Ela retorna um ponteiro para a estrutura responsável por armazenar os dados dos processos na memória.
	*/
	struct __processlist_t *Aux=(struct __processlist_t *)malloc(sizeof(struct __processlist_t));
	Aux->Queued=Aux->Running=Aux->Finished=NULL;
	Aux->QueueN=Aux->RunningN=Aux->FinishedN=0;
	Aux->ProcManType=org_type;
	return Aux;
}

char AddProcess(int PID, int TickStart, int TickCount, int Priority, struct __processlist_t *PL){
	/*
	* Esta função adiciona à fila de espera de 'PL' o processo de PID 'PID', prioridade 'Priority',
	* 	que se inicia no tempo 'TickStart' e dura 'TickCount' ciclos.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	PL->Queued=(struct __process_t *)realloc(PL->Queued,sizeof(struct __process_t)*(PL->QueueN+1));
	PL->Queued[PL->QueueN].PID=PID;
	PL->Queued[PL->QueueN].TickStart=TickStart;
	PL->Queued[PL->QueueN].TickCount=TickCount;
	PL->Queued[PL->QueueN].Priority=PL->Queued[PL->QueueN].RunningPoint=Priority;
	PL->QueueN++;
	return 1;
}

char ProcessExist(int PID, struct __processlist_t *PL){
	/*
	* Essa função verifica se já existe na fila de espera de 'PL' um processo de PID 'PID'.
	*
	* Ela retorna -1 em caso de erros, 0 se não existir e 1 se existir.
	*/
	if(PL==NULL)	return -1;
	int i;
	for(i=0;i<PL->QueueN;i++)
		if(PL->Queued[i].PID==PID)
			return 1;
	return 0;
}

char IncludeProcessesFrom(FILE *FStream, struct __processlist_t *PL){
	/*
	* Esta função lê de 'FStream' uma lista de processos e adiciona todos à lista de espera de 'PL', para aguardarem sua hora de serem executados.
	*
	* Ela retorna -1 em caso de erros, e o número de processos adicionados em caso de sucesso.
	*/
	if(PL==NULL)	return -1;
	int ScanCount,PID,TickStart,TickCount,Priority,N=0;
	if(PL->ProcManType==proc_round_robin){ // A leitura será de uma lista do tipo Round Robin ou FIFO?
		while( (ScanCount=fscanf(FStream,"%d %d %d %d",&PID,&TickStart,&TickCount,&Priority))!=EOF && ScanCount==4 ){ // Para cada processo lido da entrada (Round Robin)...
			while(ProcessExist(PID,PL)==1){ // Enquanto o PID for repetido...
				PID++; // Incrementar PID.
			}
			if(Priority==0)	Priority=6; // Caso em que a prioridade é de tempo real.
			AddProcess(PID,TickStart,TickCount,Priority,PL); // Adicionar processo à LP.
			N++;
		}
	}else{
		while( (ScanCount=fscanf(FStream,"%d %d %d",&PID,&TickStart,&TickCount))!=EOF && ScanCount==3 ){ // Para cada processo lido da entrada (FIFO)...
			while(ProcessExist(PID,PL)==1){ // Enquanto o PID for repetido...
				PID++; // Incrementar PID.
			}
			AddProcess(PID,TickStart,TickCount,1,PL); // Adicionar processo, porém com prioridade fixa sempre (qualquer uma, no caso).
			N++;
		}
	}
	return N;
}

char HeapifyProcessNode(int i, struct __processlist_t *PL){
	/*
	* Esta função é responsável por manter a Max-Heap da forma "PAI MAIOR QUE FILHOS".
	* Dessa maneira, esta função ajuda a organizar a Heap para selecionarmos quais processos serão executados.
	*
	* Na literatura, é conhecida como Max-Heapify ou Min-Heapify.
	* Nesse caso, ela age como Max-Heap quando leva a prioridade em consideração (porque prioridade 4 > prioridade 3, por exemplo),
	* 	mas age como Min-Heap quando as prioridades são iguais e analisamos o PID (porque PID 7 < PID 9, por exemplo).
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	int MaxKey = i, LeftKey = 2*i+1, RightKey = 2*i+2;
	struct __process_t Aux;
	if(LeftKey<PL->RunningN && (PL->Running[LeftKey].RunningPoint>PL->Running[MaxKey].RunningPoint
		|| (PL->Running[LeftKey].RunningPoint==PL->Running[MaxKey].RunningPoint && PL->Running[LeftKey].PID<PL->Running[MaxKey].PID)))
		MaxKey=LeftKey;
	if(RightKey<PL->RunningN && (PL->Running[RightKey].RunningPoint>PL->Running[MaxKey].RunningPoint
		|| (PL->Running[RightKey].RunningPoint==PL->Running[MaxKey].RunningPoint && PL->Running[RightKey].PID<PL->Running[MaxKey].PID)))
		MaxKey=RightKey;
	if(MaxKey!=i){
		Aux=PL->Running[i];
		PL->Running[i]=PL->Running[MaxKey];
		PL->Running[MaxKey]=Aux;
		HeapifyProcessNode(MaxKey,PL);
	}
	return 1;
}

char BuildProcessHeap(struct __processlist_t *PL){
	/*
	* Esta função constrói a nossa Heap.
	* Ela deve ser chamada toda vez que um novo processo se inicia, toda vez que um processo acaba,
	* 	e toda vez que alteramos as chaves que identificam a ordem de execução dos processos.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	int i;
	for(i=PL->RunningN/2-1;i>=0;i--)
		HeapifyProcessNode(i,PL);
	return 1;
}

char StartProcess(int i,struct __processlist_t *PL){
	/*
	* Essa função inicia o processo 'i'.
	* Ela retira o processo 'i' da lista de processos em espera, e adiciona à lista de processos em execução.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	if(i<0 || i>=PL->QueueN)	return 0;
	int j;
	PL->Running=(struct __process_t *)realloc(PL->Running,sizeof(struct __process_t)*(PL->RunningN+1));
	PL->Running[PL->RunningN++]=PL->Queued[i];
	for(j=i+1;j<PL->QueueN;j++)
		PL->Queued[j-1]=PL->Queued[j];
	PL->Queued=(struct __process_t *)realloc(PL->Queued,sizeof(struct __process_t)*(--PL->QueueN));
	return 1;
}

char KillProcess(int i,struct __processlist_t *PL){
	/*
	* Essa função mata o processo 'i'.
	* Ela retira o processo 'i' da lista de processos em execução, e adiciona à lista de processos finalizados.
	* Ela deve ser chamada toda vez que um processo termina.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	if(i<0 || i>=PL->RunningN)	return 0;
	int j;
	PL->Finished=(struct __process_t *)realloc(PL->Finished,sizeof(struct __process_t)*(PL->FinishedN+1));
	PL->Finished[PL->FinishedN++]=PL->Running[i];
	for(j=i+1;j<PL->RunningN;j++)
		PL->Running[j-1]=PL->Running[j];
	PL->Running=(struct __process_t *)realloc(PL->Running,sizeof(struct __process_t)*(--PL->RunningN));
	return 1;
}

int StartProcesses(int Tick,struct __processlist_t *PL){
	/*
	* Essa função verifica se existem novos processos que vão se iniciar no tempo 'Tick'.
	* Se existirem, ela inicia tais.
	*
	* Ela retorna -1 em caso de erros, e a maior prioridade de um processo adicionado em caso de sucesso.
	*/
	if(PL==NULL)	return -1;
	int i,R=0;
	for(i=PL->QueueN-1;i>=0;i--){
		if(PL->Queued[i].TickStart<=Tick){
			if(PL->Queued[i].Priority>R)	R=PL->Queued[i].Priority;
			StartProcess(i,PL);

		}
	}
	return R;
}

char RunProcesses(struct __processlist_t *PL){
	/*
	* Essa função executa todos os processos de 'PL' até que não sobre nenhum.
	* Além disso, ela já organiza a lista de processos finalizados.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	int i,R=0, ExecutionTick=0; // O tempo começa em zero.
	if(PL->ProcManType == proc_round_robin){ // Vai ser Round Robin ou FIFO?
		while (PL->RunningN>0 || PL->QueueN>0) { // Round Robin.
			BuildProcessHeap(PL); // Construir ou reconstruir Heap.
			if(PL->RunningN>0 && (PL->Running[0].RunningPoint==-1 || PL->Running[0].RunningPoint==5) ){ // Já executou todo um ciclo?
				for(i=0;i<PL->RunningN;i++){ // Sim, retornar processos ao seu padrão.
					PL->Running[i].RunningPoint=PL->Running[i].Priority;
				}
				continue;
			}
			R=StartProcesses(ExecutionTick,PL); // Iniciar possíveis processos nesse tempo.
			if(R>0){ // Teve algum processo que iniciou?
				if(R-1>PL->Running[0].Priority){ // Sim. Ele forçará retornar um nível de prioridade?
					for(i=0;i<PL->RunningN;i++) // Sim. Retornar um nível de prioridade.
						if(PL->Running[i].Priority <= R)
							PL->Running[i].RunningPoint=PL->Running[i].Priority;
							continue;
				}else if(ExecutionTick>1 && R>PL->Running[0].Priority){ // Sim. Ele forçará executar os processos novamente?
					for(i=1;i<PL->RunningN;i++) // Sim. Executar os processos novamente.
						PL->Running[i].RunningPoint=-1;
				}
			}
			if(PL->RunningN>0){ // Há algum processo executando?
				PL->Running[0].TickCount--; // Sim. Executar então.
				if(PL->Running[0].TickCount<1) { // O processo terminou todas suas operações?
					PL->Running[0].TickCount=ExecutionTick; // Sim. Armazenar quando ele terminou e matar ele.
					KillProcess(0,PL);
				}else{
					if(PL->Running[0].Priority==6)	PL->Running[0].RunningPoint=5; // Não. Apenas não executar mais nesse ciclo.
					else	PL->Running[0].RunningPoint=-1;
				}
			}
			ExecutionTick++; // Incrementar tempo atual.
		}
	}else{
		while (PL->RunningN>0 || PL->QueueN>0) { // FIFO.
			if(PL->RunningN<1){ // Vamos adicionar um processo agora?
				StartProcesses(R++, PL); // Sim. Adicionar processo.
				if(R<=ExecutionTick)	continue;
			}
			BuildProcessHeap(PL); // Construir ou reconstruir Heap.
			if(PL->RunningN>0){ // Há algum processo executando?
				PL->Running[0].TickCount--; // Sim. Executar então.
				if(PL->Running[0].TickCount<1) { // O processo terminou todas suas operações?
					PL->Running[0].TickCount=ExecutionTick; // Sim. Armazenar quando ele terminou e matar ele.
					KillProcess(0,PL);
				}
			}
			ExecutionTick++; // Incrementar tempo atual.
		}
	}
	return 1;
}

char PrintResults(FILE *FStream,struct __processlist_t *PL){
	/*
	* Essa função imprime em 'FStream' a lista de processos finalizados.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	if(PL->Finished==NULL)	return 0;
	int i;
	for(i=0;i<PL->FinishedN;i++){
		fprintf(FStream,"%d %d\n",PL->Finished[i].PID,PL->Finished[i].TickCount);
	}
	return 1;
}

char DestroySession(struct __processlist_t *PL){
	/*
	* Essa função remove da memória a estrutura responsável por armazenar informações dos processos 'PL'.
	*
	* Ela retorna 0 em caso de erros, e 1 em caso de sucesso.
	*/
	if(PL==NULL)	return 0;
	if(PL->Queued!=NULL)	free(PL->Queued);
	if(PL->Running!=NULL)	free(PL->Running);
	if(PL->Finished!=NULL)	free(PL->Finished);
	free(PL);
	return 1;
}
