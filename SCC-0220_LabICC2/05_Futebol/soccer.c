#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <super_string.h>



/*
 *	~         SOCCER        ~
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
		fprintf(FStream,"%d) %s %dp, %dj (%d-%d-%d), %dsg (%d-%d)\n",i+1,Soc->Teams[i].Name,Soc->Teams[i].Points,Soc->Teams[i].VictoryCount+Soc->Teams[i].DrawCount+Soc->Teams[i].DefeatCount,Soc->Teams[i].VictoryCount,Soc->Teams[i].DrawCount,Soc->Teams[i].DefeatCount,Soc->Teams[i].GoalCount-Soc->Teams[i].ReceivedGoalCount,Soc->Teams[i].GoalCount,Soc->Teams[i].ReceivedGoalCount);
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



