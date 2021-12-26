#include <stdlib.h>
#include <stdio.h>

/*
	Matheus Carvalho Raimundo
	mcarvalhor@usp.br
*/


typedef enum {false = 0, true, undefinedbool} bool;

typedef struct{
	bool **Lab;
	char StartPoint;
	char EndPoint;
	int Width;
	int Height;
} lab;


bool CheckLab(lab Obj,int X,int Y){
	if(X<0 || Y<0 || X>=Obj.Width || Y>=Obj.Height)
		return false;
	if(Obj.Lab[Y][X]!=true)
		return false;
	else if(Obj.Lab[Y][X]==true)
		Obj.Lab[Y][X]=undefinedbool;
	if(Obj.EndPoint=='N' && Y==0){
		return true;
	}else if(Obj.EndPoint=='S' && Y==Obj.Height-1){
		return true;
	}else if(Obj.EndPoint=='L' && X==Obj.Width-1){
		return true;
	}else if(Obj.EndPoint=='O' && X==0){
		return true;
	}
	if(X+1<Obj.Width && Obj.Lab[Y][X+1]==true && CheckLab(Obj,X+1,Y)==true){
		return true;
	}else if(Y+1<Obj.Height && Obj.Lab[Y+1][X]==true && CheckLab(Obj,X,Y+1)==true){
		return true;
	}else if(X-1>=0 && Obj.Lab[Y][X-1]==true && CheckLab(Obj,X-1,Y)==true){
		return true;
	}else if(Y-1>=0 && Obj.Lab[Y-1][X]==true && CheckLab(Obj,X,Y-1)==true){
		return true;
	}
	if(Obj.Lab[Y][X]==undefinedbool)
		Obj.Lab[Y][X]=true;
	return false;
}


int main(int argc, char ** argv){
	lab Obj;
	int i,j,R;
	bool Possible=false;
	scanf("%c %c %d %d",&Obj.StartPoint,&Obj.EndPoint,&Obj.Height,&Obj.Width);
	Obj.Lab=(bool **)malloc(sizeof(bool *)*Obj.Width);
	for(i=0;i<Obj.Height;i++){
		Obj.Lab[i]=(bool *)malloc(sizeof(bool)*Obj.Width);
		for(j=0;j<Obj.Width;j++){
			scanf("%d",&R);
			Obj.Lab[i][j]=(R==0) ? true : false;
		}
	}
	if(Obj.StartPoint=='N'){
		for(i=0;i<Obj.Width;i++)
			if(CheckLab(Obj,i,0)==true){
				Possible=true;
				break;
			}
	}else if(Obj.StartPoint=='S'){
		for(i=0;i<Obj.Width;i++)
			if(CheckLab(Obj,i,Obj.Height-1)==true){
				Possible=true;
				break;
			}
	}else if(Obj.StartPoint=='L'){
		for(i=0;i<Obj.Height;i++)
			if(CheckLab(Obj,Obj.Width-1,i)==true){
				Possible=true;
				break;
			}
	}else{ // Obj.StartPoint == 'O'
		for(i=0;i<Obj.Height;i++)
			if(CheckLab(Obj,0,i)==true){
				Possible=true;
				break;
			}
	}
	if(Possible==true){
		printf("Existe saida %c - %c.\n",Obj.StartPoint,Obj.EndPoint);
	}else{
		printf("Nao existe saida %c - %c.\n",Obj.StartPoint,Obj.EndPoint);
	}
	for(i=0;i<Obj.Height;i++)
		free(Obj.Lab[i]);
	free(Obj.Lab);
	return EXIT_SUCCESS;
}