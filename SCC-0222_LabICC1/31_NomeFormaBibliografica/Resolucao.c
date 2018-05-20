#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
Matheus Carvalho Raimundo
mcarvalhor@usp.br
*/

void ParaMaiusculo(char *str){
	int i;
	for(i=0;i<strlen(str);i++){
		if((str[i]>=97) && (str[i]<=122)){
			str[i]=str[i]-32;
		}
	}
}

void AntesDe(char *str,char *before,int position){
	int i;
	for(i=0;i<position;i++){
		before[i]=str[i];
	}
}

void DepoisDe(char *str,char *after,int position){
	int i=strlen(str);
	for(i=i;i>position;i--){
		after[i-position-1]=str[i];
	}
}

int UltimaPosicaoDe(char *str,char ch){
	int i,last_position=-1;
	for(i=0;i<strlen(str);i++){
		if(str[i]==ch){
			last_position=i;
		}
	}
	return last_position;
}

int main(){
	char FullName[5000];
	fgets(FullName,sizeof(FullName),stdin);
	FullName[strlen(FullName)-1]='\0';
	char AntesDeChar[UltimaPosicaoDe(FullName,' ')+1],DepoisDeChar[strlen(FullName)-UltimaPosicaoDe(FullName,' ')];
	AntesDeChar[UltimaPosicaoDe(FullName,' ')]='\0';
	DepoisDeChar[strlen(FullName)-UltimaPosicaoDe(FullName,' ')-1]='\0';
	AntesDe(FullName,AntesDeChar,UltimaPosicaoDe(FullName,' '));
	DepoisDe(FullName,DepoisDeChar,UltimaPosicaoDe(FullName,' '));
	ParaMaiusculo(DepoisDeChar);
	printf("%s, %s",DepoisDeChar, AntesDeChar);
	return EXIT_SUCCESS;
}
