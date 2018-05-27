#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>




/*
 *	~        FUTEBOL        ~
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


char *SS_ReadUntil(char *String,char Ch,FILE *FStream){
	/*
	* Esta função lê de 'FStream' uma string até chegar em um caractere 'Ch'. Ela não inclui 'Ch' na string gerada, mas inclui o caracter NULL "\0".
	* 	- Se 'String' for igual a NULL, a função vai alocar na memória uma string e retornar o endereço para tal.
	* 	- Se 'String' for diferente de NULL, a função vai modificar a própria 'String'.
	*	  CUIDADO: Nesse caso, a função pode e vai sobrescrever indefinidamente 'String', podendo inclusive ultrapassar os limites de tal, ocasionando um erro de segmentação (Out of Bounds).
	*
	* Ela retorna um ponteiro para a string alocada ou modificada.
	*/
	int n=0;
	char R,*Aux=String;
	if(Aux==NULL){
		Aux=(char*)malloc(sizeof(char)*1);
		while( (R=getc(FStream))!=EOF && R!=Ch){
			Aux=(char *)realloc(Aux,sizeof(char)*n+2);
			Aux[n++]=R;
		}
	}else
		while( (R=getc(FStream))!=EOF && R!=Ch)	Aux[n++]=R;
	Aux[n]='\0';
	if(R!=EOF)	fseek(FStream,-1,SEEK_CUR);
	return Aux;
}

int SS_ReadAllWhites(FILE *FStream){
	/*
	* Esta função avança o ponteiro de 'FStream' até um ponto em que não existam mais caracteres considerados "white-space".
	*
	* Ela retorna o número de caracteres pulados para chegar até tal, ou -1 em caso de erros.
	*/
	char R;
	int n=0;
	while( (R=getc(FStream))!=EOF && isspace(R) )	n++;
	if(R==EOF)	return -1;
	fseek(FStream,-1,SEEK_CUR);
	return n;
}

char SS_InsensitiveCmp(char *A,char *B){
	/*
	* Esta função compara 'A' com 'B' em formato "case-insensitive", ou seja, sem levar em consideração a diferença entre maiúsculos e minúsculos.
	*
	* Ela retorna -1 para 'A' maior que 'B', 0 se forem iguais, e 1 para 'B' maior que 'A'.
	*/
	int i,NA=strlen(A),NB=strlen(B);
	char *StrA=(char *)malloc(sizeof(char)*NA),*StrB=(char *)malloc(sizeof(char)*NB);
	for(i=0;i<NA;i++)	StrA[i]=tolower(A[i]);
	for(i=0;i<NB;i++)	StrB[i]=tolower(B[i]);
	i=strcmp(StrA,StrB);
	free(StrA);
	free(StrB);
	if(i>0)	return 1;
	else if(i<0)	return -1;
	else	return 0;
}



struct __soccer_game_t { // Estrutura de cada jogo.
	int TeamA,TeamB;
	int PointsA,PointsB;
};

struct __soccer_team_t { // Estrutura de cada equipe.
	char Name[50];
	int Points,VictoryCount,DrawCount,DefeatCount,GoalCount,ReceivedGoalCount;
};

struct __soccer_tour_t { // Estrutura de um torneio.
	char Name[150];
	int TeamCount,GameCount;
	struct __soccer_team_t *Teams;
	struct __soccer_game_t *Games;
};



struct __soccer_tour_t *S_NewFrom(FILE *FStream){
	/*
	* Esta função cria um novo torneio a partir de 'FStream'.
	*/
	struct __soccer_tour_t *Aux=(struct __soccer_tour_t *)malloc(sizeof(struct __soccer_tour_t));
	char Str[200];
	int i,j;
	SS_ReadAllWhites(FStream);
	SS_ReadUntil(Aux->Name,'\n',FStream);
	scanf("%d",&Aux->TeamCount);
	Aux->Teams=(struct __soccer_team_t *)malloc(sizeof(struct __soccer_team_t)*Aux->TeamCount);
	for(i=0;i<Aux->TeamCount;i++){
		SS_ReadAllWhites(FStream);
		SS_ReadUntil(Aux->Teams[i].Name,'\n',FStream);
		Aux->Teams[i].Points=Aux->Teams[i].VictoryCount=Aux->Teams[i].DrawCount=Aux->Teams[i].DefeatCount=Aux->Teams[i].GoalCount=Aux->Teams[i].ReceivedGoalCount=0;
	}
	scanf("%d",&Aux->GameCount);
	Aux->Games=(struct __soccer_game_t *)malloc(sizeof(struct __soccer_game_t)*Aux->GameCount);
	for(i=0;i<Aux->GameCount;i++){
		SS_ReadAllWhites(FStream);
		SS_ReadUntil(Str,'#',FStream);
		for(j=0;j<Aux->TeamCount;j++)
			if(strcmp(Aux->Teams[j].Name,Str)==0){
				Aux->Games[i].TeamA=j;
				break;
			}
		scanf("%*c%d%*c%d%*c",&Aux->Games[i].PointsA,&Aux->Games[i].PointsB);
		SS_ReadUntil(Str,'\n',FStream);
		for(j=0;j<Aux->TeamCount;j++)
			if(strcmp(Aux->Teams[j].Name,Str)==0){
				Aux->Games[i].TeamB=j;
				break;
			}
	}
	return Aux;
}

char S_Calc(struct __soccer_tour_t *Soc){
	/*
	* Esta função calcula todos os gols de 'Soc' e salva o resultado de cada equipe.
	*
	* Ela retorna 1 em caso de sucesso, e 0 em caso de erros.
	*/
	if(Soc==NULL)	return 0;
	int i;
	for(i=0;i<Soc->GameCount;i++){ // Para cada jogo...
		Soc->Teams[Soc->Games[i].TeamA].GoalCount+=Soc->Games[i].PointsA; // Calcular pontuação das equipes participantes.
		Soc->Teams[Soc->Games[i].TeamB].GoalCount+=Soc->Games[i].PointsB;
		Soc->Teams[Soc->Games[i].TeamA].ReceivedGoalCount+=Soc->Games[i].PointsB;
		Soc->Teams[Soc->Games[i].TeamB].ReceivedGoalCount+=Soc->Games[i].PointsA;
		if(Soc->Games[i].PointsA>Soc->Games[i].PointsB){ // A venceu B...
			Soc->Teams[Soc->Games[i].TeamA].Points+=3; // Mais 3 pontos para A.
			Soc->Teams[Soc->Games[i].TeamA].VictoryCount++;
			Soc->Teams[Soc->Games[i].TeamB].DefeatCount++;
		}else if(Soc->Games[i].PointsA<Soc->Games[i].PointsB){ // B venceu A...
			Soc->Teams[Soc->Games[i].TeamB].Points+=3; // Mais 3 pontos para B.
			Soc->Teams[Soc->Games[i].TeamB].VictoryCount++;
			Soc->Teams[Soc->Games[i].TeamA].DefeatCount++;
		}else{ // Empate...
			Soc->Teams[Soc->Games[i].TeamA].Points+=1; // Mais 1 ponto para A.
			Soc->Teams[Soc->Games[i].TeamB].Points+=1; // Mais 1 ponto para B.
			Soc->Teams[Soc->Games[i].TeamA].DrawCount++;
			Soc->Teams[Soc->Games[i].TeamB].DrawCount++;
		}
	}
	return 1;
}

char S_Sort(struct __soccer_tour_t *Soc){
	/*
	* Esta função ordena as equipes de 'Soc' com base nos requisitos.
	*
	* Ela retorna 1 em caso de sucesso, e 0 em caso de erros.
	*/
	if(Soc==NULL)	return 0;
	struct __soccer_team_t R;
	int i,j,k;
	k=Soc->TeamCount/2;
	// Shell Sort
        while(k>0){
                for(j=k;j<Soc->TeamCount;j++){
                        R=Soc->Teams[j];
                        i=j;
                        while( i>=k && ( Soc->Teams[i-k].Points<R.Points
			|| (Soc->Teams[i-k].Points==R.Points && Soc->Teams[i-k].VictoryCount<R.VictoryCount)
			|| (Soc->Teams[i-k].Points==R.Points && Soc->Teams[i-k].VictoryCount==R.VictoryCount && Soc->Teams[i-k].GoalCount-Soc->Teams[i-k].ReceivedGoalCount<R.GoalCount-R.ReceivedGoalCount)
			|| (Soc->Teams[i-k].Points==R.Points && Soc->Teams[i-k].VictoryCount==R.VictoryCount && Soc->Teams[i-k].GoalCount-Soc->Teams[i-k].ReceivedGoalCount==R.GoalCount-R.ReceivedGoalCount && Soc->Teams[i-k].GoalCount<R.GoalCount)
			|| (Soc->Teams[i-k].Points==R.Points && Soc->Teams[i-k].VictoryCount==R.VictoryCount && Soc->Teams[i-k].GoalCount-Soc->Teams[i-k].ReceivedGoalCount==R.GoalCount-R.ReceivedGoalCount && Soc->Teams[i-k].GoalCount==R.GoalCount && Soc->Teams[i-k].VictoryCount+Soc->Teams[i-k].DrawCount+Soc->Teams[i-k].DefeatCount>R.VictoryCount+R.DrawCount+R.DefeatCount)
			|| (Soc->Teams[i-k].Points==R.Points && Soc->Teams[i-k].VictoryCount==R.VictoryCount && Soc->Teams[i-k].GoalCount-Soc->Teams[i-k].ReceivedGoalCount==R.GoalCount-R.ReceivedGoalCount && Soc->Teams[i-k].GoalCount==R.GoalCount && Soc->Teams[i-k].VictoryCount+Soc->Teams[i-k].DrawCount+Soc->Teams[i-k].DefeatCount==R.VictoryCount+R.DrawCount+R.DefeatCount && SS_InsensitiveCmp(Soc->Teams[i-k].Name,R.Name)>0) ) ){
                                Soc->Teams[i]=Soc->Teams[i-k];
                                i-=k;
                        }
                        Soc->Teams[i]=R;
                }
                k/=2;
        }
	return 1;
}

char S_Print(struct __soccer_tour_t *Soc,FILE *FStream){
	/*
	* Esta função imprime as equipes de 'Soc' e seus resultados em 'FStream'.
	*
	* Ela retorna 1 em caso de sucesso, e 0 em caso de erros.
	*/
	if(Soc==NULL)	return 0;
	int i;
	fprintf(FStream,"%s\n",Soc->Name);
	for(i=0;i<Soc->TeamCount;i++){
		fprintf(FStream,"%d) %s %dp, %dg (%d-%d-%d), %dgd (%d-%d)\n",i+1,Soc->Teams[i].Name,Soc->Teams[i].Points,Soc->Teams[i].VictoryCount+Soc->Teams[i].DrawCount+Soc->Teams[i].DefeatCount,Soc->Teams[i].VictoryCount,Soc->Teams[i].DrawCount,Soc->Teams[i].DefeatCount,Soc->Teams[i].GoalCount-Soc->Teams[i].ReceivedGoalCount,Soc->Teams[i].GoalCount,Soc->Teams[i].ReceivedGoalCount);
	}
	return 1;
}

char S_Destroy(struct __soccer_tour_t *Soc){
	/*
	* Esta função limpa 'Soc' da memória.
	*
	* Ela retorna 1 em caso de sucesso, e 0 em caso de erros.
	*/
	if(Soc==NULL)	return 0;
	free(Soc->Teams);
	free(Soc->Games);
	free(Soc);
	return 1;
}



typedef struct __soccer_tour_t SOCCER_TOUR;

int main(int argc, char **argv){
	int i,N;
	SOCCER_TOUR *Aux;
	scanf("%d",&N); // Quantos torneios serão?
	for(i=0;i<N;i++){ // Para cada torneio...
		if(i>0)	printf("\n");
		Aux=S_NewFrom(stdin); // Ler torneio da entrada padrão.
		S_Calc(Aux); // Calcular pontuação de cada equipe.
		S_Sort(Aux); // Ordenar da forma correta.
		S_Print(Aux,stdout); // Imprimir na saída padrão.
		S_Destroy(Aux); // Limpar da memória.
	}
	return EXIT_SUCCESS;
}



