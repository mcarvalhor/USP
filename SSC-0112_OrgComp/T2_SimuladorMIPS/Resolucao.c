#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* POSIX */
#include <semaphore.h>
#include <pthread.h>
#include <sched.h>



/*
*
*	== Simulador de uma CPU MIPS Multiciclo de 32 bits ==
*
*	Organização de Computadores Digitais I	(SSC-0112 2018.1)
*	Professor Paulo Sérgio Lopes de Souza
*	ICMC - Universidade de São Paulo
*
*
*	OBS: O projeto foi desenvolvido utilizando POSIX Threads. Para compilar utilizando o GCC é necessário
*	incluir "-pthread". É compatível também com a versão padrão do C. Foi desenvolvido e testado nos
*	ambientes Linux Mint e Canonical Ubuntu, apresentando-se totalmente funcional.
*
*	 _______ _______ _______
*	|   |   |               \
*	|   |   |      \    |___|
*	|   |    \      |   |
*	|_______ _______|___|
*
*/



/* == Definições de pré-compilação == */

#define MEM_SIZE 512 /* Tamanho da memória: 512 bytes */

#define RegDst0 0
#define RegDst1 1
#define RegWrite 2
#define ALUSrcA 3
#define ALUSrcB0 4
#define ALUSrcB1 5
#define ALUOp0 6
#define ALUOp1 7
#define PCSource0 8
#define PCSource1 9
#define PCWriteCond 10
#define PCWrite 11
#define IorD 12
#define MemRead 13
#define MemWrite 14
#define BNE 15
#define IRWrite 16
#define MemtoReg0 17
#define MemtoReg1 18

#define BIT_CONTROLE(bit) (1 << bit) /* Para obter o bit de determinado Sinal de Controle */
#define CONTROLE(bit) ((SinaisDeControle & BIT_CONTROLE(bit)) ? -1 : 0) /* Para obter o valor de determinado Sinal de Controle */



/* == Threads para os elementos lógicos do caminho de dados == */
pthread_t
	pthread_ControlUnity,
	pthread_PCRegister,
	pthread_MemoryAddressMux,
	pthread_Memory,
	pthread_InstructionRegister,
	pthread_MemoryDataRegister,
	pthread_WriteRegisterMux,
	pthread_WriteDataMux,
	pthread_Registers,
	pthread_SignExtend,
	pthread_ShiftLeft,
	pthread_ARegister,
	pthread_BRegister,
	pthread_ALUAMux,
	pthread_ALUBMux,
	pthread_ALUControl,
	pthread_ShiftLeftConcatenate,
	pthread_ALU,
	pthread_ALUOutRegister, 
	pthread_PCMux,
	pthread_BNEMux,
	pthread_PortaAND,
	pthread_PortaOR;

/* Semáfaros que determinaram a lógica dos elementos no caminho de dados */
sem_t
	sem_ControlUnity,
	sem_PCRegister,
	sem_MemoryAddressMux,
	sem_Memory,
	sem_InstructionRegister,
	sem_MemoryDataRegister,
	sem_WriteRegisterMux,
	sem_WriteDataMux,
	sem_Registers,
	sem_SignExtend,
	sem_ShiftLeft,
	sem_ARegister,
	sem_BRegister,
	sem_ALUAMux,
	sem_ALUBMux,
	sem_ALUControl,
	sem_ShiftLeftConcatenate,
	sem_ALU,
	sem_ALUOutRegister, 
	sem_PCMux,
	sem_BNEMux,
	sem_PortaAND,
	sem_PortaOR,
	sem_SemafaroContador;

/* Registradores ao longo do caminho de dados */
int
	PC,
	IR,
	MDR,
	A,
	B,
	ALUOut,
	/* Variáveis que armazenam o valor que será atribuído apenas no próximo ciclo: como se fossem os barramentos */
	_PC,
	_IR,
	_MDR,
	_A,
	_B,
	_ALUOut,
	/* Barramentos que representam a saída de alguns Muxes, Extensor de Sinal, Shift Left ou Portas Lógicas */
	_MemoryAddressMux,
	_WriteRegisterMux,
	_WriteDataMux,
	_SignExtend,
	_ShiftLeft,
	_ALUAMux,
	_ALUBMux,
	_ALUControl,
	_ShiftLeftConcatenate,
	_ALUZero,
	_BNEMux,
	_PortaAND,
	_PortaOR;



/* == Sinais de controle == */
unsigned int SinaisDeControle;
unsigned int _SinaisDeControle; /* Último ciclo válido (para impressão apenas) */

/* Indica para as Threads se elas podem rodar, ou se o programa será encerrado */
char Run;

/* Contador de Clock (extra) */
int ClockCounter;

/* Indica se houve algum erro na execução de um elemento lógico: ULA, instrução inválida, ... */
char ProgramError;

/* Banco de Registradores */
int RegistersArray[32];

/* Memória Principal, endereçada a byte (char) */
char MainMemory[MEM_SIZE];



/* == Definição de funções == */
void LiberarSemafaros();
void ImprimirTudoSair(int, int);
void *ControlUnity(void *);
void *PCRegister(void *);
void *MemoryAddressMux(void *);
void *Memory(void *);
void *InstructionRegister(void *);
void *MemoryDataRegister(void *);
void *WriteRegisterMux(void *);
void *WriteDataMux(void *);
void *Registers(void *);
void *SignExtend(void *);
void *ShiftLeft(void *);
void *ARegister(void *);
void *BRegister(void *);
void *ALUAMux(void *);
void *ALUBMux(void *);
void *ALUControl(void *);
void *ShiftLeftConcatenate(void *);
void *ALU(void *);
void *ALUOutRegister(void *);
void *PCMux(void *);
void *BNEMux(void *);
void *PortaAND(void *);
void *PortaOR(void *);



/* == MAIN == */

int main(int argc, char **argv) {
	FILE *BinaryFile;
	int PCEnd;
	PCEnd = ClockCounter = ProgramError = 0;

	/* Boas vindas */
	printf("Bem-vind@ ao nosso programa.\n");
	printf("Grupo:\n");
	printf("\tGabriel Seiji Matsumoto (Nº 10295332)\n");
	printf("\tGustavo Kobayashi (Nº 10295711)\n");
	printf("\tMatheus Carvalho Raimundo (Nº 10369014)\n");
	printf("\tMauricio Murakami (Nº 10295346)\n\n");

	if(argc != 2) { /* Erro: não passou o binário "code.bin" */
		printf("Você passou os argumentos do programa de forma incorreta.\n");
		printf("É necessário passar o binário (\"code.bin\") para ser executado:\n\t%s [BINARIO]\n\n", argv[0]);
		return EXIT_FAILURE;
	}

	/* Iniciar todos os registradores, "barramentos", sinais de controle e memória com zero */
	memset(&PC, 0, sizeof(int));
	memset(&IR, 0, sizeof(int));
	memset(&MDR, 0, sizeof(int));
	memset(&A, 0, sizeof(int));
	memset(&B, 0, sizeof(int));
	memset(&ALUOut, 0, sizeof(int));
	memset(&_PC, 0, sizeof(int));
	memset(&_IR, 0, sizeof(int));
	memset(&_MDR, 0, sizeof(int));
	memset(&_A, 0, sizeof(int));
	memset(&_B, 0, sizeof(int));
	memset(&_ALUOut, 0, sizeof(int));
	memset(&_MemoryAddressMux, 0, sizeof(int));
	memset(&_WriteRegisterMux, 0, sizeof(int));
	memset(&_WriteDataMux, 0, sizeof(int));
	memset(&_SignExtend, 0, sizeof(int));
	memset(&_ShiftLeft, 0, sizeof(int));
	memset(&_ALUAMux, 0, sizeof(int));
	memset(&_ALUBMux, 0, sizeof(int));
	memset(&_ALUControl, 0, sizeof(int));
	memset(&_ShiftLeftConcatenate, 0, sizeof(int));
	memset(&_ALUZero, 0, sizeof(int));
	memset(&_BNEMux, 0, sizeof(int));
	memset(&_PortaAND, 0, sizeof(int));
	memset(&_PortaOR, 0, sizeof(int));
	memset(&SinaisDeControle, 0, sizeof(int));
	memset(&RegistersArray, 0, sizeof(int) * 32);
	memset(&MainMemory, 0, sizeof(char) * MEM_SIZE);

	/* Abrir arquivo de entrada para ler programa */
	BinaryFile = fopen(argv[1], "r");
	if(!BinaryFile) {
		printf("Não foi possível abrir o arquivo binário \"%s\".\n", argv[1]);
		printf("Verifique se este realmente existe e se o programa tem permissão para ler tal.\n\n");
		return EXIT_FAILURE;
	}

	printf("Lendo binário e colocando na memória...\n");

	/* Ler arquivo de entrada (programa) e colocar na memória */
	while(fscanf(BinaryFile, "%u", (unsigned int *) &MainMemory[PCEnd]) != EOF) {
		PCEnd += sizeof(int);
	}

	fclose(BinaryFile);
	printf("Leitura completa. Executando binário...\n");

	/* Inicializar todos os semáfaros */
	sem_init(&sem_ControlUnity, 0, 0);
	sem_init(&sem_PCRegister, 0, 0);
	sem_init(&sem_MemoryAddressMux, 0, 0);
	sem_init(&sem_Memory, 0, 0);
	sem_init(&sem_InstructionRegister, 0, 0);
	sem_init(&sem_MemoryDataRegister, 0, 0);
	sem_init(&sem_WriteRegisterMux, 0, 0);
	sem_init(&sem_WriteDataMux, 0, 0);
	sem_init(&sem_Registers, 0, 0);
	sem_init(&sem_SignExtend, 0, 0);
	sem_init(&sem_ShiftLeft, 0, 0);
	sem_init(&sem_ARegister, 0, 0);
	sem_init(&sem_BRegister, 0, 0);
	sem_init(&sem_ALUAMux, 0, 0);
	sem_init(&sem_ALUBMux, 0, 0);
	sem_init(&sem_ALUControl, 0, 0);
	sem_init(&sem_ShiftLeftConcatenate, 0, 0);
	sem_init(&sem_ALU, 0, 0);
	sem_init(&sem_ALUOutRegister, 0, 0);
	sem_init(&sem_PCMux, 0, 0);
	sem_init(&sem_BNEMux, 0, 0);
	sem_init(&sem_PortaAND, 0, 0);
	sem_init(&sem_PortaOR, 0, 0);

	/* Criar Threads */
	pthread_create(&pthread_ControlUnity, NULL, ControlUnity, NULL);
	pthread_create(&pthread_PCRegister, NULL, PCRegister, NULL);
	pthread_create(&pthread_MemoryAddressMux, NULL, MemoryAddressMux, NULL);
	pthread_create(&pthread_Memory, NULL, Memory, NULL);
	pthread_create(&pthread_InstructionRegister, NULL, InstructionRegister, NULL);
	pthread_create(&pthread_MemoryDataRegister, NULL, MemoryDataRegister, NULL);
	pthread_create(&pthread_WriteRegisterMux, NULL, WriteRegisterMux, NULL);
	pthread_create(&pthread_WriteDataMux, NULL, WriteDataMux, NULL);
	pthread_create(&pthread_Registers, NULL, Registers, NULL);
	pthread_create(&pthread_SignExtend, NULL, SignExtend, NULL);
	pthread_create(&pthread_ShiftLeft, NULL, ShiftLeft, NULL);
	pthread_create(&pthread_ARegister, NULL, ARegister, NULL);
	pthread_create(&pthread_BRegister, NULL, BRegister, NULL);
	pthread_create(&pthread_ALUAMux, NULL, ALUAMux, NULL);
	pthread_create(&pthread_ALUBMux, NULL, ALUBMux, NULL);
	pthread_create(&pthread_ALUControl, NULL, ALUControl, NULL);
	pthread_create(&pthread_ShiftLeftConcatenate, NULL, ShiftLeftConcatenate, NULL);
	pthread_create(&pthread_ALU, NULL, ALU, NULL);
	pthread_create(&pthread_ALUOutRegister, NULL, ALUOutRegister, NULL);
	pthread_create(&pthread_PCMux, NULL, PCMux, NULL);
	pthread_create(&pthread_BNEMux, NULL, BNEMux, NULL);
	pthread_create(&pthread_PortaAND, NULL, PortaAND, NULL);
	pthread_create(&pthread_PortaOR, NULL, PortaOR, NULL);

	Run = 1;
	while(PC < PCEnd) { /* Enquanto o programa não tiver finalizado */
		LiberarSemafaros();
		ClockCounter++;
	}

	ImprimirTudoSair(0, 0); /* O programa terminou e nenhum erro foi encontrado */

	return EXIT_SUCCESS;
}



/* == Funções == */

void LiberarSemafaros() {
	/*
	 * O trabalho foi implementado utilizando threads e de forma paralela.
	 * Contudo, no caminho de dados nem tudo é "executado" desta maneira.
	 * Para isso, criamos esta função, que controla o que precisa ser
	 *   executado antes do que.
	 * 
	 * Por exemplo: A ULA faz o resultado de uma operação baseando-se em
	 *   um sinal que indique qual a operação, e sob quais valores ela
	 *   vai operar. Sendo assim, ela PRECISA que os dois Muxes e a
	 *   Unidade de Controle sejam executados primeiro, para que
	 *   possa colocar o resultado da operação de forma correta.
	 * Esta função sabe disso, e só vai executar a ULA quando seus sinais
	 *   e Muxes estiverem prontos.
	 * 
	 * Ela faz isso usando um Semáfaro Contador e a função do POSIX "sched_yield".
	 * Com "sched_yield", podemos mandar nossa thread principal para o fim da fila
	 *   de prioridade de processos do sistema operacional até que todas as outras
	 *   threads que precisamos tenham sido executadas.
	 */
	int Aux;

	/*
	 * Primeiramente, os Registradores são processados, pois estes não têm dependência
	 *   linear de sinais ou bits de outros elementos no caminho de dados.
	 * São os primeiros elementos executados, no exato momento da subida do clock.
	 */
	sem_init(&sem_SemafaroContador, 0, 7);
	sem_post(&sem_PCRegister);
	sem_post(&sem_InstructionRegister);
	sem_post(&sem_MemoryDataRegister);
	sem_post(&sem_Registers); /* Banco de registradores: fazer possível escrita */
	sem_post(&sem_ARegister);
	sem_post(&sem_BRegister);
	sem_post(&sem_ALUOutRegister);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Aguarda todos acima serem executados */
	sem_destroy(&sem_SemafaroContador);

	/* Erro: acesso inválido ao banco de registradores */
	if(ProgramError) {
		ImprimirTudoSair(4, _WriteRegisterMux);
	}

	/*
	 * Agora vamos executar a Unidade de Controle, a Extensão de Sinal e o Shift Left Concatenado com PC.
	 * Isso porque estes dois últimos dependem de IR, que já foi executado.
	 */
	sem_init(&sem_SemafaroContador, 0, 3);
	sem_post(&sem_ControlUnity);
	sem_post(&sem_SignExtend);
	sem_post(&sem_ShiftLeftConcatenate);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Aguarda execução dos acima */
	sem_destroy(&sem_SemafaroContador);

	/* Erro: instrução inválida */
	if(ProgramError) {
		ImprimirTudoSair(1, IR >> 26);
	}

	/*
	 * Agora que a Unidade de Controle foi executada, tudo o que depende de seus sinais pode ser executado.
	 * Isso inclue alguns Muxes e a Unidade de Controle secundária.
	 * 
	 * Além disso, podemos executar o Shift Left do endereço relativo, visto que já foi feita a Extensão de Sinal.
	 */
	sem_init(&sem_SemafaroContador, 0, 6);
	sem_post(&sem_MemoryAddressMux);
	sem_post(&sem_WriteRegisterMux);
	sem_post(&sem_WriteDataMux);
	sem_post(&sem_ShiftLeft);
	sem_post(&sem_ALUAMux);
	sem_post(&sem_ALUControl);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Aguarda execução dos acima */
	sem_destroy(&sem_SemafaroContador);

	/*
	 * Agora podemos ler da Memória, porque o Mux já selecionou o endereço correto, podemos ler
	 *   do banco de Registradores, e podemos executar o Mux da segunda entrada da ULA.
	 */
	sem_init(&sem_SemafaroContador, 0, 3);
	sem_post(&sem_Memory);
	sem_post(&sem_Registers);
	sem_post(&sem_ALUBMux);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Aguarda execução dos acima */
	sem_destroy(&sem_SemafaroContador);

	/* Erro: acesso a endereço inválido na memória */
	if(ProgramError) {
		ImprimirTudoSair(2, _MemoryAddressMux);
	}

	/* Agora sim a ULA tem todo o necessário para funcionar (não depende de mais nada no caminho de dados) */
	sem_init(&sem_SemafaroContador, 0, 1);
	sem_post(&sem_ALU);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Espera a ULA */
	sem_destroy(&sem_SemafaroContador);

	/* Erro: operação inválida no campo de função para ULA */
	if(ProgramError) {
		ImprimirTudoSair(3, _ALUControl);
	}

	/*
	 * Agora que a ULA executou, podemos executar o Mux que escolhe o que vai escrever no PC e o Mux BNE.
	 */
	sem_init(&sem_SemafaroContador, 0, 2);
	sem_post(&sem_PCMux);
	sem_post(&sem_BNEMux);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Espera esses Muxes */

	/*
	 * Agora que o Mux do BNE foi executado, podemos fazer a porta AND.
	 */
	sem_post(&sem_SemafaroContador);
	sem_post(&sem_PortaAND);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Espera a AND */

	/*
	 * Por fim, a porta OR é executada.
	 */
	sem_post(&sem_SemafaroContador);
	sem_post(&sem_PortaOR);

	do {
		sched_yield();
		sem_getvalue(&sem_SemafaroContador, &Aux);
	} while(Aux); /* Espera a OR */
	sem_destroy(&sem_SemafaroContador);

	_SinaisDeControle = SinaisDeControle; /* Executou sem erros: ciclo válido */
}

void ImprimirTudoSair(int Status, int Details) {
	/*
	 * Esta função imprime todos os registradores e as primeiras 32 posições (palavras) da memória principal.
	 * Após, ela solicita que todas as Threads sejam encerradas e encerra o programa.
	 * 
	 * EXTRA: Nos terminais compatíveis (geralmente UNIX), vai imprimir colorido!
	 */
	char Binary[50];
	int i;

	printf("\n\n\x1b[1m\x1b[40m\x1b[33m== Programa finalizado no clock %d ==\x1b[0m\n", ClockCounter);
	printf("\x1b[5mStatus da Saída:\x1b[0m ");
	switch(Status) { /* Imprimir o tipo de código de erro (ou sucesso, no caso deste) e detalhes, se houverem */
		case 0:
			printf("(0) programa encerrado com sucesso.\n");
		break;
		case 1:
			printf("(1) término devido a tentativa de execução de instrução inválida com campo de operação de valor [%d].\n", Details);
		break;
		case 2:
			printf("(2) término devido a acesso inválido de memória no endereço [0x%x].\n", Details);
		break;
		case 3:
			printf("(3) término devido a operação inválida na unidade lógica e aritmética com campo de função de valor [%d].\n", Details);
		break;
		case 4:
			printf("(4) término devido a acesso inválido ao banco de registradores no índice [%d].\n", Details);
		break;
		default:
			printf("(-1) erro indeterminado forçou término.\n");
	}

	/* Imprimir registradores */
	printf("\nPC = %-10u\tIR = %-10u\t   MDR = %-10u\n", PC, IR, MDR);
	printf(" A = %-10u\t B = %-10u\tALUOut = %-10u\n", A, B, ALUOut);

	/* Sinais de controle (bits), na ordem do mais significativo para o menos (de acordo com a especificação *.PDF) */
	SinaisDeControle = _SinaisDeControle;
	for(i = RegDst0; i <= MemtoReg1; i++)
		Binary[MemtoReg1 - i] = (CONTROLE(i)) ? '1' : '0';
	Binary[i] = '\0';
	printf("Controle (bits) = %s (%u em decimal)\n", Binary, SinaisDeControle);

	/* Banco de registradores */
	printf("\n\x1b[1mBanco de Registradores:\x1b[0m\n");
	printf("R00(r0) = %-11d\tR08(t0) = %-11d\tR16(s0) = %-11d\tR24(t8) = %-11d\n", RegistersArray[0], RegistersArray[8], RegistersArray[16], RegistersArray[24]);
	printf("R01(at) = %-11d\tR09(t1) = %-11d\tR17(s1) = %-11d\tR25(t9) = %-11d\n", RegistersArray[1], RegistersArray[9], RegistersArray[17], RegistersArray[25]);
	printf("R02(v0) = %-11d\tR10(t2) = %-11d\tR18(s2) = %-11d\tR26(k0) = %-11d\n", RegistersArray[2], RegistersArray[10], RegistersArray[18], RegistersArray[26]);
	printf("R03(v1) = %-11d\tR11(t3) = %-11d\tR19(s3) = %-11d\tR27(k1) = %-11d\n", RegistersArray[3], RegistersArray[11], RegistersArray[19], RegistersArray[27]);
	printf("R04(a0) = %-11d\tR12(t4) = %-11d\tR20(s4) = %-11d\tR28(gp) = %-11d\n", RegistersArray[4], RegistersArray[12], RegistersArray[20], RegistersArray[28]);
	printf("R05(a1) = %-11d\tR13(t5) = %-11d\tR21(s5) = %-11d\tR29(sp) = %-11d\n", RegistersArray[5], RegistersArray[13], RegistersArray[21], RegistersArray[29]);
	printf("R06(a2) = %-11d\tR14(t6) = %-11d\tR22(s6) = %-11d\tR30(fp) = %-11d\n", RegistersArray[6], RegistersArray[14], RegistersArray[22], RegistersArray[30]);
	printf("R07(a3) = %-11d\tR15(t7) = %-11d\tR23(s7) = %-11d\tR31(ra) = %-11d\n", RegistersArray[7], RegistersArray[15], RegistersArray[23], RegistersArray[31]);

	/* Memória primária principal */
	printf("\n\x1b[1mMemória (endereçada a byte):\x1b[0m\n");
	for(i = 0; i <= 28; i += 4) {
		printf("[%03d] = %-10u\t[%03d] = %-10u\t", i, *((unsigned int *) &MainMemory[i]), i + 32, *((unsigned int *) &MainMemory[i + 32]));
		printf("[%03d] = %-10u\t[%03d] = %-10u\n", i + 64, *((unsigned int *) &MainMemory[i + 64]), i + 96, *((unsigned int *) &MainMemory[i + 96]));
	}
	printf("\n");

	/* Terminar todas as threads dos elementos lógicos no caminho de dados */
	Run = 0;
	sem_post(&sem_PCRegister);
	sem_post(&sem_InstructionRegister);
	sem_post(&sem_MemoryDataRegister);
	sem_post(&sem_Registers);
	sem_post(&sem_ARegister);
	sem_post(&sem_BRegister);
	sem_post(&sem_ALUOutRegister);
	sem_post(&sem_ControlUnity);
	sem_post(&sem_SignExtend);
	sem_post(&sem_ShiftLeftConcatenate);
	sem_post(&sem_MemoryAddressMux);
	sem_post(&sem_WriteRegisterMux);
	sem_post(&sem_WriteDataMux);
	sem_post(&sem_ShiftLeft);
	sem_post(&sem_ALUAMux);
	sem_post(&sem_ALUControl);
	sem_post(&sem_Memory);
	sem_post(&sem_ALUBMux);
	sem_post(&sem_ALU);
	sem_post(&sem_PCMux);
	sem_post(&sem_BNEMux);
	sem_post(&sem_PortaAND);
	sem_post(&sem_PortaOR);

	pthread_join(pthread_ControlUnity, NULL);
	pthread_join(pthread_PCRegister, NULL);
	pthread_join(pthread_MemoryAddressMux, NULL);
	pthread_join(pthread_Memory, NULL);
	pthread_join(pthread_InstructionRegister, NULL);
	pthread_join(pthread_MemoryDataRegister, NULL);
	pthread_join(pthread_WriteRegisterMux, NULL);
	pthread_join(pthread_WriteDataMux, NULL);
	pthread_join(pthread_Registers, NULL);
	pthread_join(pthread_SignExtend, NULL);
	pthread_join(pthread_ShiftLeft, NULL);
	pthread_join(pthread_ARegister, NULL);
	pthread_join(pthread_BRegister, NULL);
	pthread_join(pthread_ALUAMux, NULL);
	pthread_join(pthread_ALUBMux, NULL);
	pthread_join(pthread_ALUControl, NULL);
	pthread_join(pthread_ShiftLeftConcatenate, NULL);
	pthread_join(pthread_ALU, NULL);
	pthread_join(pthread_ALUOutRegister, NULL);
	pthread_join(pthread_PCMux, NULL);
	pthread_join(pthread_BNEMux, NULL);
	pthread_join(pthread_PortaAND, NULL);
	pthread_join(pthread_PortaOR, NULL);

	/* Destruir semáfaros da memória (free) */
	sem_destroy(&sem_ControlUnity);
	sem_destroy(&sem_PCRegister);
	sem_destroy(&sem_MemoryAddressMux);
	sem_destroy(&sem_Memory);
	sem_destroy(&sem_InstructionRegister);
	sem_destroy(&sem_MemoryDataRegister);
	sem_destroy(&sem_WriteRegisterMux);
	sem_destroy(&sem_WriteDataMux);
	sem_destroy(&sem_Registers);
	sem_destroy(&sem_SignExtend);
	sem_destroy(&sem_ShiftLeft);
	sem_destroy(&sem_ARegister);
	sem_destroy(&sem_BRegister);
	sem_destroy(&sem_ALUAMux);
	sem_destroy(&sem_ALUBMux);
	sem_destroy(&sem_ALUControl);
	sem_destroy(&sem_ShiftLeftConcatenate);
	sem_destroy(&sem_ALU);
	sem_destroy(&sem_ALUOutRegister);
	sem_destroy(&sem_PCMux);
	sem_destroy(&sem_BNEMux);
	sem_destroy(&sem_PortaAND);
	sem_destroy(&sem_PortaOR);

	/* Sair com código de erro ou sucesso */
	exit((Status == 0) ? EXIT_SUCCESS : EXIT_FAILURE);

}



/* == Agora começam as implementações de cada elemento lógico no caminho de dados == */

void *ControlUnity(void *args) {
	/*
	 * A Unidade de Controle foi implementada através de uma tabela verdade bit a bit.
	 */
	char CurrentState; /* Representa o estado atual. Apenas os 5 primeiros bits são necessários. */
	char _CurrentState; /* Representa o estado no próximo ciclo de clock. Apenas os 5 primeiros bits são necessários. */
	#define ESTADO(bit) ((CurrentState & (1 << bit)) ? -1 : 0) /* Para obter o valor de um bit do estado atual */
	#define OP(bit) (((IR >> 26) & (1 << bit)) ? -1 : 0) /* Para obter o valor de um bit do campo de operação de IR */
	CurrentState = _CurrentState = 0;
	while(1) {
		sem_wait(&sem_ControlUnity);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}

		CurrentState = _CurrentState;
		SinaisDeControle = 0;
		_CurrentState = 0;

		/* A partir de agora começa a tabela verdade que fizemos para cada bit que a Unidade de Controle gerencia */

		SinaisDeControle |= BIT_CONTROLE(RegDst0) & (ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(RegDst1) & (ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(RegWrite) & ((~ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(ALUSrcA) & ((ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(ALUSrcB0) & ((ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(ALUSrcB1) & ((ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(ALUOp0) & ((ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(ALUOp1) & ((ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(PCSource0) & ((ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(PCSource1) & ((ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(PCWriteCond) & ((ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(PCWrite) & ((~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(IorD) & ((ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(MemRead) & ((~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)));

		SinaisDeControle |= BIT_CONTROLE(MemWrite) & (ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(BNE) & (ESTADO(0) & ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(IRWrite) & (~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(MemtoReg0) & (~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4));

		SinaisDeControle |= BIT_CONTROLE(MemtoReg1) & ((~ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ESTADO(3) & ~ESTADO(4)));

		/* Função Próximo Estado */

		_CurrentState |= BIT_CONTROLE(0) & ((~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)));

		_CurrentState |= BIT_CONTROLE(1) & ((~ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & OP(3) & ~OP(4) & OP(5)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)));

		_CurrentState |= BIT_CONTROLE(2) & ((ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ESTADO(4)) |
								(~ESTADO(0) & ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & OP(3) & ~OP(4) & OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)));

		_CurrentState |= BIT_CONTROLE(3) & ((~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ESTADO(2) & ESTADO(3) & ~ESTADO(4)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & OP(2) & OP(3) & ~OP(4) & ~OP(5)) |
								(ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)));

		_CurrentState |= BIT_CONTROLE(4) & (ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4) & ~OP(0) & ~OP(1) & ~OP(2) & OP(3) & ~OP(4) & ~OP(5));

		/* Sinal que indica se é uma instrução válida ou não, para encerrar o programa se for inválida */

		ProgramError = (~ESTADO(0) & ~ESTADO(1) & ~ESTADO(2) & ~ESTADO(3) & ~ESTADO(4)) | (OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & OP(5)) |
								(OP(0) & OP(1) & ~OP(2) & OP(3) & ~OP(4) & OP(5)) |
								(~OP(0) & ~OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(~OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(~OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(~OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(OP(0) & OP(1) & ~OP(2) & ~OP(3) & ~OP(4) & ~OP(5)) |
								(OP(0) & ~OP(1) & OP(2) & ~OP(3) & OP(4) & ~OP(5)) |
								(~OP(0) & ~OP(1) & OP(2) & OP(3) & ~OP(4) & ~OP(5)) |
								(~OP(0) & ~OP(1) & ~OP(2) & OP(3) & ~OP(4) & ~OP(5)) |
								(OP(0) & ~OP(1) & OP(2) & ~OP(3) & ~OP(4) & ~OP(5));
		ProgramError = ~ProgramError;

		sem_trywait(&sem_SemafaroContador);
	}
}

void *PCRegister(void *args) {
	/*
	 * PC: atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_PCRegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(_PortaOR) {
			PC = _PC; /* Escreve se a Porta OR for verdadeira */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *MemoryAddressMux(void *args) {
	/*
	 * Esse é o Mux que fica entre o PC e a Memória Principal, que seleciona o endereço que
	 *   será acessado para leitura ou escrita na memória.
	 */
	while(1) {
		sem_wait(&sem_MemoryAddressMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(IorD)) {
			_MemoryAddressMux = ALUOut; /* 1 */
		} else {
			_MemoryAddressMux = PC; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *Memory(void *args) {
	/*
	 * Esse é o componente que acessa a memória fazendo leituras e escritas.
	 */
	while(1) {
		sem_wait(&sem_Memory);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if((CONTROLE(MemWrite) || CONTROLE(MemRead)) && (_MemoryAddressMux < 0 || _MemoryAddressMux >= MEM_SIZE)) {
			ProgramError = 1; /* Endereço de memória não é válido */
		} else {
			if(CONTROLE(MemWrite)) {
				memcpy(&MainMemory[_MemoryAddressMux], &B, sizeof(int)); /* Escrita */
			}
			if(CONTROLE(MemRead)) {
				memcpy(&_MDR, &MainMemory[_MemoryAddressMux], sizeof(int)); /* Leitura */
				_IR = _MDR;
			}
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *InstructionRegister(void *args) {
	/*
	 * IR: atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_InstructionRegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(IRWrite)) {
			IR = _IR; /* Escreve se o sinal IRWrite estiver ativo */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *MemoryDataRegister(void *args) {
	/*
	 * MDR, atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_MemoryDataRegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		MDR = _MDR; /* Escreve */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *WriteRegisterMux(void *args) {
	/*
	 * Esse é o Mux que fica entre o IR e o Banco de Registradores, que seleciona em qual registrador
	 *   do Banco de Registradores será feita uma determinada escrita.
	 */
	while(1) {
		sem_wait(&sem_WriteRegisterMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(RegDst1)) {
			_WriteRegisterMux = 31; /* 2 */
		} else if(CONTROLE(RegDst0)) {
			_WriteRegisterMux = (IR >> 11) & 0x1F; /* 1 */
		} else {
			_WriteRegisterMux = (IR >> 16) & 0x1F; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *WriteDataMux(void *args) {
	/*
	 * Esse é o Mux que fica entre o MDR e o Banco de Registradores, que seleciona qual dado
	 *   será escrito em um determinado registrador do Banco de Registradores.
	 */
	while(1) {
		sem_wait(&sem_WriteDataMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(MemtoReg1)) {
			_WriteDataMux = PC; /* 2 */
		} else if(CONTROLE(MemtoReg0)) {
			_WriteDataMux = MDR; /* 1 */
		} else {
			_WriteDataMux = ALUOut; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *Registers(void *args) {
	/*
	 * Esse é o componente que acessa o Banco de Registradores fazendo leituras e escritas.
	 *
	 * Na lógica sequencial, ele é "executado" duas vezes:
	 *  - uma na subida do clock, para apenas escrita do ciclo anterior;
	 *  - uma posteriormente, para apenas leituras.
	 */
	while(1) {
		sem_wait(&sem_Registers);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(RegWrite)) {
			if(_WriteRegisterMux == 0){
				ProgramError = 1; /* Erro porque não pode escrever no registrador zero */
			} else {
				RegistersArray[_WriteRegisterMux] = _WriteDataMux; /* Escrita */
			}
		}
		sem_trywait(&sem_SemafaroContador);
		sem_wait(&sem_Registers); /* Esperar de novo para hora de ler... */
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_A = RegistersArray[(IR >> 21) & 0x1F]; /* Ler para A */
		_B = RegistersArray[(IR >> 16) & 0x1F]; /* Ler para B */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *SignExtend(void *args) {
	/*
	 * Esse é o Extensor de Sinal que fica em baixo do Banco de Registradores, usado
	 *   para calcular endereço relativo a PC ou operar com imediato.
	 */
	while(1) {
		sem_wait(&sem_SignExtend);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(IR & 0x8000) {
			_SignExtend = (IR & 0xFFFF) | (-1 ^ 0xFFFF); /* Negativo, estender tudo para 1 */
		} else {
			_SignExtend = IR & 0xFFFF; /* Positivo, estender tudo para 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ShiftLeft(void *args) {
	/*
	 * Esse é o Shift Left que fica em baixo do Banco de Registradores e ao lado do
	 *   Extensor de Sinal, usado para calcular endereço relativo a PC.
	 */
	while(1) {
		sem_wait(&sem_ShiftLeft);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_ShiftLeft = _SignExtend << 2; /* Multiplicado por 4: tamanho da palavra (Byte Offset) */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ARegister(void *args) {
	/*
	 * Registrador A: atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_ARegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		A = _A; /* Escreve */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *BRegister(void *args) {
	/*
	 * Registrador B: atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_BRegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		B = _B; /* Escreve */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ALUAMux(void *args) {
	/*
	 * Esse é o Mux que fica entre o Registrador A e a ULA, que seleciona o primeiro
	 *   valor a ser operado na ULA.
	 */
	while(1) {
		sem_wait(&sem_ALUAMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(ALUSrcA)) {
			_ALUAMux = A; /* 1 */
		} else {
			_ALUAMux = PC; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ALUBMux(void *args) {
	/*
	 * Esse é o Mux que fica entre o Registrador B e a ULA, que seleciona o segundo
	 *   valor a ser operado na ULA.
	 */
	while(1) {
		sem_wait(&sem_ALUBMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(ALUSrcB1) && CONTROLE(ALUSrcB0)) {
			_ALUBMux = _ShiftLeft; /* 3 */
		} else if(CONTROLE(ALUSrcB1) && !CONTROLE(ALUSrcB0)) {
			_ALUBMux = _SignExtend; /* 2 */
		} else if(CONTROLE(ALUSrcB0)) {
			_ALUBMux = 4; /* 1 */
		} else {
			_ALUBMux = B; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ALUControl(void *args) {
	/*
	 * Essa é a Unidade de Controle Secundária, que fica em baixo da ULA e é responsável por selecionar
	 *   se a ULA fará adição, subtração, 'e', ou operação determinada no campo de função de IR.
	 *
	 * A implementação desta foi feita por tabela verdade.
	 */
	while(1) {
		sem_wait(&sem_ALUControl);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_ALUControl = (0x20 & ~CONTROLE(ALUOp1) & ~CONTROLE(ALUOp0)) | /* Soma */
						(0x22 & ~CONTROLE(ALUOp1) & CONTROLE(ALUOp0)) | /* Subtração */
						((IR & 0x3F) & CONTROLE(ALUOp1) & ~CONTROLE(ALUOp0)) | /* Campo função */
						(0x24 & CONTROLE(ALUOp1) & CONTROLE(ALUOp0)); /* And */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ShiftLeftConcatenate(void *args) {
	/*
	 * Esse é o Shift Left que fica em cima da ULA, usado para um pulo (branch)
	 *   incondicional. Ele é concatenado com conteúdo de PC.
	 */
	while(1) {
		sem_wait(&sem_ShiftLeftConcatenate);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_ShiftLeftConcatenate = (IR & 0x3FFFFFF) << 2; /* Multiplicado por 4: tamanho da palavra (Byte Offset) */
		_ShiftLeftConcatenate |= PC & ~0xFFFFFFF; /* Concatenar os bits restantes com valor de PC */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ALU(void *args) {
	/*
	 * Essa é a ULA, que vai realizar as operações aritméticas.
	 *
	 * Não foi implementada todas as operações que a ULA da arquitetura MIPS é capaz
	 *   de realizar, mas apenas as relevantes a este trabalho.
	 */
	while(1) {
		sem_wait(&sem_ALU);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		switch(_ALUControl) {
			case 0x20:
				_ALUOut = _ALUAMux + _ALUBMux; /* add */
				_ALUZero = !_ALUOut;
			break;
			case 0x22:
				_ALUOut = _ALUAMux - _ALUBMux; /* sub */
				_ALUZero = !_ALUOut;
			break;
			case 0x2A:
				_ALUOut = (_ALUAMux < _ALUBMux) ? 1 : 0; /* slt */
				_ALUZero = !_ALUOut;
			break;
			case 0x24:
				_ALUOut = _ALUAMux & _ALUBMux; /* and */
				_ALUZero = !_ALUOut;
			break;
			case 0x25:
				_ALUOut = _ALUAMux | _ALUBMux; /* or */
			break;
			default:
				if(CONTROLE(ALUOp1) && !CONTROLE(ALUOp0)) /* Tipo-R */
					ProgramError = 1; /* A ULA não é compatível com esta operação */
		}
		_ALUZero = !_ALUOut;
		sem_trywait(&sem_SemafaroContador);
	}
}

void *ALUOutRegister(void *args) {
	/*
	 * ALUOut: atualiza valor na subida do ciclo de clock.
	 */
	while(1) {
		sem_wait(&sem_ALUOutRegister);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		ALUOut = _ALUOut; /* Escreve */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *PCMux(void *args) {
	/*
	 * Esse é o Mux que fica após a ULA e o Registrador ALUOut, que seleciona o próximo
	 *   valor (endereço de instrução na memória) que PC assumirá.
	 */
	while(1) {
		sem_wait(&sem_PCMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(PCSource1) && CONTROLE(PCSource0)) {
			_PC = A; /* 3 */
		} else if(CONTROLE(PCSource1) && !CONTROLE(PCSource0)) {
			_PC = _ShiftLeftConcatenate; /* 2 */
		} else if (CONTROLE(PCSource0)) {
			_PC = ALUOut; /* 1 */
		} else {
			_PC = _ALUOut; /* 0 */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *BNEMux(void *args) {
	/*
	 * Esse é o Mux que fica acima de tudo, que seleciona o bit correto:
	 *   - se a ULA deu zero (no caso de BEQ);
	 *   - ou não (no caso de BNE).
	 */
	while(1) {
		sem_wait(&sem_BNEMux);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		if(CONTROLE(BNE)) {
			_BNEMux = (_ALUZero) ? 0 : 1; /* É BNE */
		} else {
			_BNEMux = _ALUZero; /* Não é BNE */
		}
		sem_trywait(&sem_SemafaroContador);
	}
}

void *PortaAND(void *args) {
	/*
	 * Esta porta lógica combina o Mux acima com o fato de ser mesmo BEQ ou BNE.
	 */
	while(1) {
		sem_wait(&sem_PortaAND);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_PortaAND = (CONTROLE(PCWriteCond) && _BNEMux) ? 1 : 0; /* É BEQ ou BNE */
		sem_trywait(&sem_SemafaroContador);
	}
}

void *PortaOR(void *args) {
	/*
	 * Esta porta lógica determina se a instrução em execução altera o valor de
	 *   PC nesse ciclo (para ser usado no próximo).
	 */
	while(1) {
		sem_wait(&sem_PortaOR);
		if(!Run) { /* Finalizar thread */
			return NULL;
		}
		_PortaOR = (CONTROLE(PCWrite) || _PortaAND) ? 1 : 0; /* Escreve em PC */
		sem_trywait(&sem_SemafaroContador);
	}
}


