#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stack.h>



/*
 *	~        LABYRINTH      ~
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



enum __bool_t { false=0, true }; // Pseudo-booleano.

struct __point_t { // Cada ponto do labirinto, independente se é câmara, túnel de segmento ou beco sem saída, é representado por esta estrutura.
	float x, y; // Posição.
	enum __bool_t exit,visited; // Se o ponto for câmara, indica se é uma saída do labirinto ou se já passamos por aqui, na hora de encontrar soluções.
	int links_to[8]; // O ponto pode ter túneis que o liga até 8 outros pontos.
};

struct __treasure_invert_action_t { // Essa estrutura é utilizada para indicar as ações que a câmara de tesouro solicita.
	int A,B;
};

struct __solution_path_t { // Cada caminho de solução é dado por esta estrutura.
	enum __bool_t Treasure; // Indica se o caminho passa pela câmara de tesouro.
	float GeoSum,Points; // Soma Geodésica e Pontuação do caminho.
	STACK *Path; // Pilha que representa o caminho.
};

struct __lab_t { // O labirinto como um todo é representado por esta estrutura.
	int Cs,Ct,NP,SolutionCount,T,TCount;
	struct __treasure_invert_action_t *TActions;
	struct __point_t *Points;
	struct __solution_path_t *Solutions;
};



struct __lab_t *ReadLab(FILE *FStream){
	/*
	* Esta função vai ler as entradas necessárias para formar um labirinto de 'FStream'.
	*
	* Ela retorna o labirinto formado.
	*/
	struct __lab_t *Aux=(struct __lab_t *)malloc(sizeof(struct __lab_t));
	int i,j,R,S,T,U;
	Aux->SolutionCount=Aux->TCount=0;
	Aux->Solutions=NULL;
	Aux->TActions=NULL;
	scanf("%d",&Aux->NP); // Número de pontos.
	Aux->Points=(struct __point_t *)malloc(sizeof(struct __point_t)*Aux->NP);
	for(i=0;i<Aux->NP;i++){ // Para cada ponto...
		scanf("%f %f",&Aux->Points[i].x,&Aux->Points[i].y); // Obter sua posição.
		Aux->Points[i].exit=Aux->Points[i].visited=false;
		for(j=0;j<8;j++)	Aux->Points[i].links_to[j]=0;
	}
	scanf("%d",&R); // Número de câmaras.
	for(i=0;i<R;i++){ // Para cada câmara...
		scanf("%d %d",&S,&T);
		if(T!=0 && S>0 && S <=Aux->NP)	Aux->Points[S-1].exit=true; // Se a câmara for de saída, marcar isso no ponto que representa ela.
	}
	scanf("%d",&R); // Número de segmentos.
	for(i=0;i<R;i++){ // Para cada segmento...
		scanf("%d %d",&S,&T); // Conectar os pontos correspondentes.
		if(S<0 || T<0)	U=-1;
		else	U=1;
		R=abs(R);
		S=abs(S);
		if(S>0 && S<=Aux->NP && T>0 && T<=Aux->NP){
			j=0;
			while(abs(Aux->Points[S-1].links_to[j])!=0)	j++;
			Aux->Points[S-1].links_to[j]=T*U;
			j=0;
			while(abs(Aux->Points[T-1].links_to[j])!=0)	j++;
			Aux->Points[T-1].links_to[j]=S*U;
		}
	}
	scanf("%d %d %d",&Aux->Ct,&Aux->T,&R); // Câmara do tesouro, Pontuação do tesouro e Número de ações.
	if(Aux->Ct<=0 || Aux->Ct>Aux->NP){ // Verificação de integridade da câmara de tesouro.
		Aux->Ct=0; // Câmara não é válida! Não existe tesouro nesse labirinto mais.
		Aux->T=0;
		for(i=0;i<R;i++)	scanf("%*d %*d");
	}else{
		for(i=0;i<R;i++){ // Para cada ação do tesouro...
			scanf("%d %d",&S,&T); // Registrar essa ação para uso apenas quando passar pela câmara de tesouro.
			if(S<0 || T<0)	U=-1;
			else	U=1;
			S=abs(S);
			T=abs(T);
			if(S>0 && S<=Aux->NP && T>0 && T<=Aux->NP){
				Aux->TActions=(struct __treasure_invert_action_t *)realloc(Aux->TActions,sizeof(struct __treasure_invert_action_t)*(++Aux->TCount));
				Aux->TActions[Aux->TCount-1].A=S*U;
				Aux->TActions[Aux->TCount-1].B=T*U;
			}
		}
	}
	scanf("%d",&Aux->Cs); // Câmara inicial.
	return Aux;
}

char FindSolutionsWithoutTreasure(struct __lab_t *Lab){
	/*
	* Esta função vai procurar por todas as soluções possíveis para o labirinto já formado 'Lab', SEM PASSAR PELO TESOURO.
	*
	* Ela retorna 2 em caso de sucesso, 1 caso não tenha encontrado nenhum caminho (caso Cs=Ct), e 0 em caso de erros.
	*/
	if(Lab==NULL)	return 0;
	if(Lab->Cs==Lab->Ct)	return 1; // Se a Câmara de tesouro e a Câmara inicial forem a mesma, ignorar essa função.
	STACK *Path=S_New(); // Criar uma pilha para representar o caminho.
	int i,Last=0;
	S_Push(Lab->Cs,Path); // Começa na câmara inicial "Cs".
	while(S_Size(Path)>0){ // Procurar por saídas sem passar pela câmara de tesouro.
		if(Lab->Points[S_Get(Path)-1].visited==false && Lab->Points[S_Get(Path)-1].exit==true && S_Get(Path)!=Lab->Cs){ // É saída?
			Lab->Solutions=(struct __solution_path_t *)realloc(Lab->Solutions,sizeof(struct __solution_path_t)*(++Lab->SolutionCount)); // SIM! Salvar caminho percorrido até aqui.
			Lab->Solutions[Lab->SolutionCount-1].Path=S_NewFrom(Path); // Copia esse caminho para o vetor de soluções.
			Lab->Solutions[Lab->SolutionCount-1].Treasure=false;
		}
		Lab->Points[S_Get(Path)-1].visited=true; // Marcar que estamos passando por aqui.
		for(i=0;i<8;i++)	if(Lab->Points[S_Get(Path)-1].links_to[i]==Last)	break; // Estamos vindo de onde?
		if(i>=8 || Last==0)	i=0; // Lugar nenhum porque chegamos aqui agora.
		else	i++; // Já passamos aqui e estamos voltando para pegar um portal diferente dessa vez.
		while(i<8){
			if(Lab->Points[S_Get(Path)-1].links_to[i]>0 && Lab->Points[S_Get(Path)-1].links_to[i]!=Lab->Ct && Lab->Points[Lab->Points[S_Get(Path)-1].links_to[i]-1].visited==false ){ // O portal tá trancado por algum motivo OU é um beco sem saída?
				Last=0; // Não. Vamos lá.
				S_Push(Lab->Points[S_Get(Path)-1].links_to[i],Path);
				break;
			}
			i++;
		}
		if(i>=8){ // Passamos por todos os portais?
			Lab->Points[S_Get(Path)-1].visited=false; // Sim. Vamos voltar pra trás então.
			Last=S_Pop(Path);
		}
	}
	S_Push(Lab->Cs,Path);
	Lab->Solutions=(struct __solution_path_t *)realloc(Lab->Solutions,sizeof(struct __solution_path_t)*(++Lab->SolutionCount)); // Colocar a solução trivial no vetor de soluções.
	Lab->Solutions[Lab->SolutionCount-1].Path=S_NewFrom(Path);
	Lab->Solutions[Lab->SolutionCount-1].Treasure=false;
	S_Destroy(Path);
	return 2;
}

char FindSolutionsWithTreasure(struct __lab_t *Lab){
	/*
	* Esta função vai procurar por todas as soluções possíveis para o labirinto já formado 'Lab', PASSANDO PELO TESOURO.
	* Vale lembrar que ela vai modificar o labirinto, realizando as ações solicitadas pela câmara do tesouro.
	*
	* Ela retorna 1 se for executada, e 0 em caso de erros.
	*/
	if(Lab==NULL)	return 0;
	STACK *Path=S_New(),**Paths=NULL; // Criar uma pilha para representar o caminho atual, e um vetor de pilhas para representar todos os caminhos possíveis até a câmara do tesouro.
	int i,j,Last=0,PathsCount=0;
	S_Push(Lab->Cs,Path); // Começa na câmara inicial "Cs".
	while(S_Size(Path)>0){ // Procurar por caminhos até a câmara do tesouro.
		if(Lab->Points[S_Get(Path)-1].visited==false && S_Get(Path)==Lab->Ct && S_Get(Path)!=Lab->Cs){ // É a câmara do tesouro?
			Paths=(STACK **)realloc(Paths,sizeof(STACK *)*(++PathsCount)); // SIM! Salvar caminho percorrido até aqui.
			Paths[PathsCount-1]=S_NewFrom(Path); // Copia esse caminho para o vetor de pilhas.
		}
		Lab->Points[S_Get(Path)-1].visited=true; // Marcar que estamos passando por aqui.
		for(i=0;i<8;i++)	if(Lab->Points[S_Get(Path)-1].links_to[i]==Last)	break; // Estamos vindo de onde?
		if(i>=8 || Last==0)	i=0; // Lugar nenhum porque chegamos aqui agora.
		else	i++; // Já passamos aqui e estamos voltando para pegar um portal diferente dessa vez.
		while(i<8){
			if(Lab->Points[S_Get(Path)-1].links_to[i]>0 && Lab->Points[Lab->Points[S_Get(Path)-1].links_to[i]-1].visited==false){ // O portal tá trancado por algum motivo OU é um beco sem saída?
				Last=0; // Não. Vamos lá.
				S_Push(Lab->Points[S_Get(Path)-1].links_to[i],Path);
				break;
			}
			i++;
		}
		if(i>=8){ // Passamos por todos os portais?
			Lab->Points[S_Get(Path)-1].visited=false; // Sim. Vamos voltar pra trás então.
			Last=S_Pop(Path);
		}
	}
	for(j=0;j<Lab->TCount;j++){ // Para cada Ação do tesouro, realizar.
		for(i=0;i<8;i++)	if(Lab->Points[abs(Lab->TActions[j].A)-1].links_to[i]==0 || abs(Lab->Points[abs(Lab->TActions[j].A)-1].links_to[i])==abs(Lab->TActions[j].B))	break;
		if(i<8)	Lab->Points[abs(Lab->TActions[j].A)-1].links_to[i]=Lab->TActions[j].B;
		for(i=0;i<8;i++)	if(Lab->Points[abs(Lab->TActions[j].B)-1].links_to[i]==0 || abs(Lab->Points[abs(Lab->TActions[j].B)-1].links_to[i])==abs(Lab->TActions[j].A))	break;
		if(i<8)	Lab->Points[abs(Lab->TActions[j].B)-1].links_to[i]=Lab->TActions[j].A;
	}
	if(Lab->Cs==Lab->Ct){ // Se a Câmara inicial e a Câmara de tesouro forem a mesma...
		Paths=(STACK **)realloc(Paths,sizeof(STACK *)*(++PathsCount)); // Então vamos sempre começar da própria Câmara de tesouro.
		Paths[PathsCount-1]=S_New();
		S_Push(Lab->Ct,Paths[PathsCount-1]);
	}
	for(j=0;j<PathsCount;j++){ // Há soluções DISTINTAS para CADA caminho que temos até a Câmara de tesouro. Vamos salvar TODAS que encontrarmos.
		S_Push(S_Get(Paths[j]),Path); // Agora vamos achar o caminho normalmente, mas o início será no último elemento da pilha (a própria Câmara de tesouro).
		Last=0;
		while(S_Size(Path)>0){ // Procurar por caminhos até a saída.
			if(Lab->Points[S_Get(Path)-1].visited==false && Lab->Points[S_Get(Path)-1].exit==true && S_Get(Path)!=Lab->Ct){ // É uma saída?
				Lab->Solutions=(struct __solution_path_t *)realloc(Lab->Solutions,sizeof(struct __solution_path_t)*(++Lab->SolutionCount)); // SIM! Salvar caminho percorrido até aqui, incluindo o caminho até a Câmara de tesouro.
				Lab->Solutions[Lab->SolutionCount-1].Path=S_NewFrom(Paths[j]); // Copia o caminho até a Câmara de tesouro para a solução.
				for(i=1;i<S_Size(Path);i++)	S_Push(S_GetAt(i,Path),Lab->Solutions[Lab->SolutionCount-1].Path); // Completa com o resto do caminho até a saída.
				Lab->Solutions[Lab->SolutionCount-1].Treasure=true;
			}
			Lab->Points[S_Get(Path)-1].visited=true; // Marcar que estamos passando por aqui.
			for(i=0;i<8;i++)	if(abs(Lab->Points[S_Get(Path)-1].links_to[i])==Last)	break; // Estamos vindo de onde?
			if(i>=8 || Last==0)	i=0; // Lugar nenhum porque chegamos aqui agora.
			else	i++; // Já passamos aqui e estamos voltando para pegar um portal diferente dessa vez.
			while(i<8){
				if(Lab->Points[S_Get(Path)-1].links_to[i]>0 && Lab->Points[Lab->Points[S_Get(Path)-1].links_to[i]-1].visited==false){ // O portal tá trancado por algum motivo OU é um beco sem saída?
					Last=0; // Não. Vamos lá.
					S_Push(abs(Lab->Points[S_Get(Path)-1].links_to[i]),Path);
					break;
				}
				i++;
			}
			if(i>=8){ // Passamos por todos os portais?
				Lab->Points[S_Get(Path)-1].visited=false; // Sim. Vamos voltar pra trás então.
				Last=S_Pop(Path);
			}
		}
		if(Lab->Points[Lab->Ct-1].exit==true){ // Se a Câmara de tesouro for uma saída também, colocar a solução trivial no vetor de soluções.
			Lab->Solutions=(struct __solution_path_t *)realloc(Lab->Solutions,sizeof(struct __solution_path_t)*(++Lab->SolutionCount));
			Lab->Solutions[Lab->SolutionCount-1].Path=S_NewFrom(Paths[j]);
			Lab->Solutions[Lab->SolutionCount-1].Treasure=true;
		}
		S_Destroy(Paths[j]);
	}
	free(Paths);
	S_Destroy(Path);
	return 1;
}

char SortSolutions(struct __lab_t *Lab){
	/*
	* Esta função vai calcular a soma geodésica e a pontuação dos caminhos soluções e ordenar de acordo com os requisitos solicitados.
	*
	* Ela retorna 1 se for executada, e 0 em caso de erros.
	*/
	if(Lab==NULL)	return 0;
	struct __solution_path_t R;
	int i,j,k;
	float GeoSum;
	// Primeiramente vamos calcular a soma geodésica e a pontuação.
	for(i=0;i<Lab->SolutionCount;i++){ // Para cada caminho solução...
		GeoSum=0; // Calcular a soma geodésica...
		for(j=1;j<S_Size(Lab->Solutions[i].Path);j++){
			GeoSum+=sqrt(pow(Lab->Points[S_GetAt(j,Lab->Solutions[i].Path)-1].x-Lab->Points[S_GetAt(j-1,Lab->Solutions[i].Path)-1].x,2)+pow(Lab->Points[S_GetAt(j,Lab->Solutions[i].Path)-1].y-Lab->Points[S_GetAt(j-1,Lab->Solutions[i].Path)-1].y,2));
		}
		Lab->Solutions[i].GeoSum=GeoSum;
		if(Lab->Solutions[i].Treasure==true)	Lab->Solutions[i].Points=Lab->T;
		else Lab->Solutions[i].Points=0;
		Lab->Solutions[i].Points-=GeoSum; // e a pontuação.
	}
	// Agora vamos ordenar (Shell Sort).
	k=Lab->SolutionCount/2;
	while(k>0){
		for(j=k;j<Lab->SolutionCount;j++){
			R=Lab->Solutions[j];
			i=j;
			while( i>=k && ( Lab->Solutions[i-k].Points<R.Points || (Lab->Solutions[i-k].Points==R.Points && Lab->Solutions[i-k].GeoSum<R.GeoSum) || (Lab->Solutions[i-k].Points==R.Points && Lab->Solutions[i-k].GeoSum==R.GeoSum && S_Size(Lab->Solutions[i-k].Path)>S_Size(R.Path)) || (Lab->Solutions[i-k].Points==R.Points && Lab->Solutions[i-k].GeoSum==R.GeoSum && S_Size(Lab->Solutions[i-k].Path)==S_Size(R.Path) && S_Cmp(Lab->Solutions[i-k].Path,R.Path)<0) ) ){
				Lab->Solutions[i]=Lab->Solutions[i-k];
				i-=k;
			}
			Lab->Solutions[i]=R;
		}
		k/=2;
	}
	return 1;
}

char PrintSolutions(struct __lab_t *Lab,FILE *FStream){
	/*
	* Esta função vai imprimir todas as soluções encontradas para o labirinto 'Lab' de forma organizada em 'FStream'.
	*
	* Ela retorna 1 se for executada, e 0 em caso de erros.
	*/
	if(Lab==NULL)	return 0;
	int i,j;
	printf("%d %d\n",Lab->Ct,Lab->T);
	for(i=0;i<Lab->SolutionCount;i++){
		fprintf(FStream,"%d",S_Size(Lab->Solutions[i].Path));
		for(j=0;j<S_Size(Lab->Solutions[i].Path);j++){
			fprintf(FStream," %d",S_GetAt(j,Lab->Solutions[i].Path));
		}
		fprintf(FStream," %d %d\n",(int)round(Lab->Solutions[i].GeoSum),(int)round(Lab->Solutions[i].Points));
	}
	return 1;
}

char DestroyLab(struct __lab_t *Lab){
	/*
	* Esta função vai limpar da memória todo o labirinto 'Lab'.
	*
	* Ela retorna 1 se for executada, e 0 em caso de erros.
	*/
	if(Lab==NULL)	return 0;
	int i;
	for(i=0;i<Lab->SolutionCount;i++)	S_Destroy(Lab->Solutions[i].Path);
	free(Lab->TActions);
	free(Lab->Points);
	free(Lab->Solutions);
	free(Lab);
	return 1;
}
