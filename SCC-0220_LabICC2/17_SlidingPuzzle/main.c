#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SlidingPuzzle.h>



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



#define MAX_D 5



int main(int argc, char **argv){
	PUZZLE *SlidPuz=New_Puzzle(stdin, MAX_D);
	char R;

	while( (R=getchar()) != EOF && R!='X' ){
		if(R=='A'){
			if(Move_Puzze_Up(SlidPuz)==1)	continue;
		}else if(R=='L'){
			if(Move_Puzze_Left(SlidPuz)==1)	continue;
		}else if(R=='B'){
			if(Move_Puzze_Down(SlidPuz)==1)	continue;
		}else if(R=='R'){
			if(Move_Puzze_Right(SlidPuz)==1)	continue;
		}
		printf("This puzzle has no final configuration.\n");
		return EXIT_SUCCESS;
	}

	Print_Puzzle(stdout, SlidPuz);

	Destroy_Puzzle(SlidPuz);

	return EXIT_SUCCESS;
}


