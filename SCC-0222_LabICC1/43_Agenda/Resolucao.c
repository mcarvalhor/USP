#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

typedef struct Time_Stamp{
	char Dia;
	char Mes;
	unsigned short Ano;
	char Hora;
	char Minuto;
	char Segundo;
} DATE;

typedef struct Calendar_Activity{
	DATE Data_Hora;
	char Nome[100];
} EVENTO;

int main(){
	int i,N,R;
	scanf("%d",&N);
	EVENTO Calendario[N];
	for(i=0;i<N;i++){
		scanf("%d",&R); //Dia
		Calendario[i].Data_Hora.Dia=(char)R;
		scanf("%d",&R); //Mês
		Calendario[i].Data_Hora.Mes=(char)R;
		scanf("%d",&R); //Ano
		Calendario[i].Data_Hora.Ano=(unsigned short)R;
		scanf("%d",&R); //Hora
		Calendario[i].Data_Hora.Hora=(char)R;
		scanf("%d",&R); //Minuto
		Calendario[i].Data_Hora.Minuto=(char)R;
		scanf("%d",&R); //Segundo
		Calendario[i].Data_Hora.Segundo=(char)R;
		getchar();
		fgets(Calendario[i].Nome,100,stdin);
		Calendario[i].Nome[strlen(Calendario[i].Nome)-1]='\0';
	}
	for(i=0;i<N;i++){
		printf("%02d/%02d/%04d - %02d:%02d:%02d\n%s\n",(int)Calendario[i].Data_Hora.Dia,(int)Calendario[i].Data_Hora.Mes,(int)Calendario[i].Data_Hora.Ano,(int)Calendario[i].Data_Hora.Hora,(int)Calendario[i].Data_Hora.Minuto,(int)Calendario[i].Data_Hora.Segundo,Calendario[i].Nome);
	}
	return EXIT_SUCCESS;
}
