#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <cstdint>

#define SIZE 25



/*
 *      ==   Jan-Ken-Puzzle PD  ==
 *
 *      Nomes:
 *       Amira Gayatri El Assad Hempel (Nº 10262631)
 *       Eric Ribeiro Augusto (Nº 10295242)
 *       Matheus Carvalho Raimundo (Nº 10369014)
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/



/* == STRUCTURE DEFINITION == */

/* This structure represents any possible solution. */
typedef struct solution_t{
	int i,j;
	int piece;
} solution_t;

typedef std::unordered_map<uint_fast64_t, int_fast64_t> boardmap_t;

typedef struct data {
	solution_t* solvedArray;
	int_fast64_t ways;
	int solutions;
	int depth;
} data;

typedef struct stBoard {
	char board[SIZE/2][SIZE/2];
	int row, col;
	int counter;
} stBoard;

typedef struct move {
	int b[2];
	int e[2];
} move;



/* == DATA DEFINITION == */
boardmap_t boardMap;
long long int runBoardtimes, islandstimes,findWaystimes;



/* == FUNCTION DEFINITION == */
int valid(int, int);
uint_fast64_t mat2int64(stBoard);
int_fast64_t findWays(uint_fast64_t);
int floodBoard(char board[SIZE/2][SIZE/2], int, int);
bool islandsBoard(stBoard, int, int);
int_fast64_t runBoard(data *, stBoard, move);
void sortSolutions(solution_t *, int);



/* == MAIN == */
int main(int argc, char **argv) {
	int i, j, read;
	stBoard b;
	move m;
	data d;
	
	d.ways = 0;
	d.depth = 0;
	d.solutions = 0;
	b.counter = 0;
	d.solvedArray = (solution_t *) malloc((SIZE * 3) * sizeof(solution_t));

	for (i = 0; i < SIZE/2; ++i) {
		for (j = 0; j < SIZE/2; ++j) {
			b.board[i][j] = 0;
		}
	}
	
	for (i = 0; i < SIZE*3; ++i) {
		d.solvedArray[i].i = 0;
		d.solvedArray[i].j = 0;
		d.solvedArray[i].piece = 0;
	}

	/* Reads from stdin the initial state of the board */
	scanf("%d %d", &b.row, &b.col);
	for (i = 0; i < b.row; ++i) {
		for (j = 0; j < b.col; ++j) {
			scanf(" %d", &read);
			b.board[i][j] = (char) read;
			if(b.board[i][j] > 0) {
				b.counter++;
				m.e[0]=i;
				m.e[1]=j;
			}
		}
	}
	m.b[0]= -1;
	
	/* Calls recursive algorithm */
	runBoard(&d, b, m);

	/* Sorts set of solutions */
	sortSolutions(d.solvedArray, d.solutions);

	/* Prints set of solutions (result) */
	printf("%ld\n", d.ways);
	printf("%d\n", d.solutions);
	for (i = 0; i < d.solutions; ++i) {
		printf("%d %d %d\n", d.solvedArray[i].i+1, d.solvedArray[i].j+1, d.solvedArray[i].piece);
	}

	free(d.solvedArray);

	return EXIT_SUCCESS;
}



/* == FUNCTION IMPLEMENTATION == */

/* This functions checks weather a move is valid or not */
int valid(int b, int e) {
	/* It returns true if valid, false otherwise */
	if((b==1 && e==2)
		|| (b==2 && e==3)
		|| (b==3 && e==1))
		return 1;
	else
		return 0;
}

/* This function converts 'nxn matrix' to 'uint_fast64_t', under base 4 */
uint_fast64_t mat2int64(stBoard b) {
	uint_fast64_t base4, matrix64;
	int i, j;

	base4 = 1;
	matrix64 = 0;
	for (i = 0; i < b.row ; ++i) {
		for (j = 0; j < b.col; ++j) {
			/* Adds 0-3 no. in mat mult. by power of 4 */
			matrix64 += b.board[i][j] * base4;
			base4 *= 4;
		}
	}

	return matrix64;
}

/* This function finds the board value in unordered map.
	If found, it returns number of solutions;
	if not found, it returns -1 */
int_fast64_t findWays(uint_fast64_t board_id) {
	boardmap_t::const_iterator got;
	
	findWaystimes++;
	got = boardMap.find(board_id);
	if(got == boardMap.end())
		return -1;
	else
		return got->second;
}

/* This function returns the number of conected cells */
int floodBoard(char board[SIZE/2][SIZE/2], int p, int q) {
	int floodCount;
	
	floodCount = 1;
	board[p][q]=0;
	if(p>0 && board[p-1][q]>0)
		floodCount += floodBoard(board,p-1,q);
	if(q>0 && board[p][q-1]>0)
		floodCount += floodBoard(board,p,q-1);
	if(p<SIZE/2-1 && board[p+1][q]>0)
		floodCount += floodBoard(board,p+1,q);	
	if(q<SIZE/2-1 && board[p][q+1]>0)
		floodCount += floodBoard(board,p,q+1);

	return floodCount;
}

/* this function returns true if there are islands on the board,
	false otherwise. */
bool islandsBoard(stBoard b, int p, int q) {
	stBoard copy;
	
	islandstimes++;
	copy = b;
	
	if(floodBoard(copy.board,p,q) != b.counter)
		return true;
	else
		return false;
}

/* This function iterates over the board looking for a valid movement.
	If any valid movement is found, the algorithm is recursively executed */
int_fast64_t runBoard(data *d, stBoard b, move m) {
	uint_fast64_t board_id;
	int_fast64_t waysFound;
	int i, j, count, last;
	
	/* If valid, moves m */
	if(m.b[0] != -1) {
		b.board[m.e[0]][m.e[1]] = b.board[m.b[0]][m.b[1]];
		b.board[m.b[0]][m.b[1]] = 0;
		b.counter--;
	}

	/* If board is already solved, increases the value of nways 
		and returns the nways found */
	board_id = mat2int64(b);
	waysFound = findWays(board_id);
	if(waysFound >= 0){
		d->ways += waysFound;
		return waysFound;
	}

	/* if any islands is found, returns 0 */
	if(islandsBoard(b, m.e[0], m.e[1])){
		boardMap.insert ({board_id, 0});
		return 0; 
	}

	/* Until there are no valid moves anymore:
		mark the movement and recursively call algorithm;
		find the next valid moves */
	count = 0;
	waysFound = 0;
	last = 0;
	for (i = 0; i < b.row; ++i) {
		for (j = 0; j < b.col; ++j) {
			/* Checks l, r, u and d */
			if(b.board[i][j] != 0) {
				count ++;
				last = b.board[i][j];
				
				/* Left (l) */
				if(j!=0) {
					if(valid(b.board[i][j], b.board[i][j-1])) {
						m.b[0] = i;
						m.b[1] = j;
						m.e[0] = i;
						m.e[1] = j-1;	
						waysFound += runBoard(d, b, m);
					}
				}
				
				/* Right (r) */
				if(j!= b.col-1) {
					if(valid(b.board[i][j], b.board[i][j+1])) {
						m.b[0] = i;
						m.b[1] = j;
						m.e[0] = i;
						m.e[1] = j+1;
						waysFound += runBoard(d, b, m);
					}
				}
				
				/* Up (u) */
				if(i!=0) {
					if(valid(b.board[i][j], b.board[i-1][j])) {
						m.b[0] = i;
						m.b[1] = j;
						m.e[0] = i-1;
						m.e[1] = j;
						waysFound += runBoard(d, b, m);
					}
				}

				/* Down (d) */
				if(i!= b.row-1) {
					if(valid(b.board[i][j], b.board[i+1][j])) {
						m.b[0] = i;
						m.b[1] = j;
						m.e[0] = i+1;
						m.e[1] = j;
						waysFound += runBoard(d, b, m);					}
				}
			}
		}
		if(count == b.counter)
			break;
	}

	/* If there is only one piece on the board, then the algorithm has finished its execution */
	if(count == 1) {
		/* Adds this solution to the set of solutions */
		d->solvedArray[d->solutions].i = m.e[0];
		d->solvedArray[d->solutions].j = m.e[1];
		d->solvedArray[d->solutions].piece = last;
		d->solutions++;
		d->ways++;
		waysFound = 1;
	}

	/* Adds solved board to map */
	boardMap.insert({board_id, waysFound});
	return waysFound;
}

/* This function sorts the set of solutions */
void sortSolutions(solution_t *s, int numSols) {
	solution_t aux;
	int i, j;
	
	for (i = 1; i < numSols; ++i) {
		aux = s[i]; 
		for (j = i-1; j >= 0; --j) {
			if(s[j].i < aux.i) {
				break;
			} else if (s[j].i == aux.i) {
				if(s[j].j < aux.j) {
					break;
				} else if (s[j].j == aux.j) {
					if(s[j].piece < aux.piece) {
						break;
					}
				}
			}
			s[j+1] = s[j];
		}
		s[j+1] = aux;
	}
}


