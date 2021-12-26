#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*
 Matheus Carvalho Raimundo
 mcarvalhor@usp.br
*/

typedef struct {
	char key[100];
	int value;
} array;

void Sort(array *, int);
void strfix(char *);

int main(int argc, char **argv){
	int i,N;
	char R[100],FileName[100];
	FILE *FStream;
	array *Words;
	scanf("%s %d",FileName,&N);
	Words=(array *)malloc(sizeof(array)*N);
	for(i=0;i<N;i++){
		scanf("%s",Words[i].key);
		strfix(Words[i].key);
		Words[i].value=0;
	}
	FStream=fopen(FileName,"r");
	if(FStream!=NULL){
		while(feof(FStream)==0){
			fscanf(FStream,"%s ",R);
			strfix(R);
			for(i=0;i<N;i++)
				if(strcmp(R,Words[i].key)==0)
					Words[i].value++;
		}
		Sort(Words,N);
		for(i=0;i<N;i++){
			printf("%s %d\n",Words[i].key,Words[i].value);
		}
		fclose(FStream);
	}
	free(Words);
	return EXIT_SUCCESS;
}


void Sort(array *ArrayVector,int N){
	int i,j;
	array R;
	for(i=0;i<N;i++){
		j=i;
		while(j>0 && strcmp(ArrayVector[j-1].key,ArrayVector[j].key)>0 ){
			R=ArrayVector[j-1];
			ArrayVector[j-1]=ArrayVector[j];
			ArrayVector[j]=R;
			j--;
		}
	}
}


void strfix(char *String){
	int i=0,j;
	while( !(String[i]>='A' && String[i]<='Z') && !(String[i]>='a' && String[i]<='z') && String[i]!='\0' ){
		j=i;
		while(String[j]!='\0'){
			String[i+j]=String[i+j+1];
			j++;
		}
		i++;
	}
	for(i=strlen(String)-1;i>=0;i--){
		if( (String[i]>='A' && String[i]<='Z') || (String[i]>='a' && String[i]<='z') ){
			break;
		}else{
			String[i]='\0';
		}
	}
	i=0;
	while(String[i]!='\0'){
		String[i]=tolower(String[i]);
		i++;
	}
}
