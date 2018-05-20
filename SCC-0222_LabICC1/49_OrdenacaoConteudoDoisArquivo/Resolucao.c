#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

typedef struct user_data {
	char CPF[15];
	char Nome[100];
	int Idade;
} USUARIO;

USUARIO **ReadUsersFrom(char *,int *,USUARIO **);

int main(){
	char File1[50],File2[50];
	USUARIO **UserData=NULL,*Rc;
	int i,j,N=0;
	fgets(File1,sizeof(File1),stdin);
	fgets(File2,sizeof(File2),stdin);
	File1[strlen(File1)-1]='\0';
	if(File2[strlen(File2)-1]=='\n'){
		File2[strlen(File2)-1]='\0';
	}
	UserData=ReadUsersFrom(File1,&N,UserData);
	UserData=ReadUsersFrom(File2,&N,UserData);
	for(i=0;i<N;i++){
		for(j=i+1;j<N;j++){
			if(strcmp(UserData[i]->CPF,UserData[j]->CPF)>0){
				Rc=UserData[i];
				UserData[i]=UserData[j];
				UserData[j]=Rc;
			}
		}
		printf("%s\n%s\n%d\n",UserData[i]->CPF,UserData[i]->Nome,UserData[i]->Idade);
		free(UserData[i]);
	}
	free(UserData);
	return EXIT_SUCCESS;
}

USUARIO **ReadUsersFrom(char *FilePath,int *n,USUARIO **UserOld){
	FILE *FStream=fopen(FilePath,"r");
	USUARIO **UserVet=UserOld;
	char Line[500];
	int N=*n;
	if(FStream){
		while(!feof(FStream)){
			fgets(Line,100,FStream);
			UserVet=(USUARIO **)realloc(UserVet,sizeof(USUARIO *)*(N+1));
			UserVet[N]=(USUARIO *)malloc(sizeof(USUARIO));
			strncpy(UserVet[N]->CPF,Line,14);
			UserVet[N]->CPF[14]='\0';
			fgets(UserVet[N]->Nome,100,FStream);
			UserVet[N]->Nome[strlen(UserVet[N]->Nome)-1]='\0';
			fscanf(FStream,"%d ",&UserVet[N]->Idade);
			N++;
		}
	}
	fclose(FStream);
	*n=N;
	return UserVet;
}
