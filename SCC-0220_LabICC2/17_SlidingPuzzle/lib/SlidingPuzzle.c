#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>



/*
 *	~    Sliding Puzzle     ~
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



struct __slidingpuzzle_t {
	char **Matrix;
	unsigned long D;
	long BlankI, BlankJ;
};



struct __slidingpuzzle_t *New_Puzzle(FILE *FStream, unsigned long D){
	if(D < 1)	return NULL;
	struct __slidingpuzzle_t *Aux = (struct __slidingpuzzle_t *)malloc(sizeof(struct __slidingpuzzle_t));
	unsigned long i, j;
	char R;
	Aux->Matrix = (char **)malloc(sizeof(char *) * D);
	Aux->BlankI = Aux->BlankJ = -1;
	Aux->D = D;
	for(i=0; i<D; i++){
		Aux->Matrix[i] = (char *)malloc(sizeof(char)*D);
		for(j=0; j<D; j++){
			Aux->Matrix[i][j]=getc(FStream);
			if(Aux->Matrix[i][j]==' '){
				Aux->BlankI=i;
				Aux->BlankJ=j;
			}
		}
		while( (R=getc(FStream)) != EOF && R != '\n') { }
	}
	return Aux;
}

void Move_Puzzle_Swap(char *A, char *B){
	if(A==NULL || B==NULL)	return;
	char R=*B;
	*B=*A;
	*A=R;
}

int Move_Puzze_Up(struct __slidingpuzzle_t *P){
	if(P==NULL)	return -1;
	if(P->BlankI <= 0)	return 0;
	Move_Puzzle_Swap(&P->Matrix[P->BlankI][P->BlankJ], &P->Matrix[P->BlankI-1][P->BlankJ]);
	P->BlankI--;
	return 1;
}

int Move_Puzze_Left(struct __slidingpuzzle_t *P){
	if(P==NULL)	return -1;
	if(P->BlankJ <= 0)	return 0;
	Move_Puzzle_Swap(&P->Matrix[P->BlankI][P->BlankJ], &P->Matrix[P->BlankI][P->BlankJ-1]);
	P->BlankJ--;
	return 1;
}

int Move_Puzze_Down(struct __slidingpuzzle_t *P){
	if(P==NULL)	return -1;
	if(P->BlankI >= P->D-1)	return 0;
	Move_Puzzle_Swap(&P->Matrix[P->BlankI][P->BlankJ], &P->Matrix[P->BlankI+1][P->BlankJ]);
	P->BlankI++;
	return 1;
}

int Move_Puzze_Right(struct __slidingpuzzle_t *P){
	if(P==NULL)	return -1;
	if(P->BlankJ >= P->D-1)	return 0;
	Move_Puzzle_Swap(&P->Matrix[P->BlankI][P->BlankJ], &P->Matrix[P->BlankI][P->BlankJ+1]);
	P->BlankJ++;
	return 1;
}

int Print_Puzzle(FILE *FStream, struct __slidingpuzzle_t *P){
	if(P==NULL)	return 0;
	unsigned long i, j;
	for(i=0;i<P->D;i++){
		for(j=0;j<P->D;j++)
			fprintf(FStream, "%c", P->Matrix[i][j]);
		fprintf(FStream, "\n");
	}
	return 1;
}

void Destroy_Puzzle(struct __slidingpuzzle_t *P){
	if(P==NULL)	return;
	unsigned long i;
	for(i=0;i<P->D;i++)
		free(P->Matrix[i]);
	free(P->Matrix);
	free(P);
}


