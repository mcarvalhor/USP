#include <stdlib.h>
#include <iostream>
#include <set>

using namespace std;



/*
 *      ==    Jan-Ken-Puzzle   ==
 *
 *      Nomes:
 *       Amira Gayatri El Assad Hempel (Nº 10262631)
 *       Matheus Carvalho Raimundo (Nº 10369014)
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/

/*
 * == Explicação do código ==
 *
 * Para solucionar este problema, fizemos um algoritmo backtracking "solve".
 * Este algoritmo vai percorrer todas as cédulas do tabuleiro verificando
 * 	possíveis movimentos, e chamando a recursão nestes casos.
 * Utilizamos um vetor "moves" para estes movimentos, pois dá para usar um
 * 	único "for" ao invés de vários "ifs" para cada um dos quatro movimentos.
 *
 * Utilizamos da STL (estrutura de dados padrão) "set" do C++:
 * 	O set (ou conjunto) permite inserir elementos sem repetir (ele já verifica
 *  	possíveis repetições) e já mantém os elementos ordenados.
 *  Usamos o set para manter as resoluções do tabuleiro, pois ele é perfeito
 *  	para tal fim.
 *
 * O código é compatível com C++ padrão ansi e não possui 'leaks' de memória.
 *
 * Obs.: Movemos todas as implementações de funções para após a "main" para
 * 	melhor organização.
*/



/* A linha abaixo é usada para facilitar identifição das cédulas no código */
enum { nothing = 0, rock, scissors, paper };

/* Possíveis movimentos no eixo cartesiano (sem contar diagonais) */
int moves[][2] = {
	{-1, 0},
	{0, -1},
	{1, 0},
	{0, 1}
};



/* A estrutura abaixo representa uma resolução para o dado tabuleiro */
typedef struct solution_t {
	int x, y; /* Coordenadas */
	char type; /* Tipo de peça que "venceu" */

	bool operator < (const solution_t& cmpObj) const {
		/* C++ STL: usado para comparar resoluções (evitar repetição e ordenar) */
		if(x == cmpObj.x) {
			if(y == cmpObj.y)
				return (type < cmpObj.type);
			return (y < cmpObj.y);
		}
		return (x < cmpObj.x);
	}

} Solution;

/* A estrutura abaixo representa um tabuleiro */
typedef struct gameBoard_t {
	int r, c; /* Número de linhas "R" e de colunas "C" */
	char **puzzle; /* Tabuleiro com as cédulas. O tipo 'char' é usado para preservar memória */
	set<Solution> solutions; /* Conjunto de resoluções (sem repetição) para dado tabuleiro. Começa vazio */
	int solutionCounter; /* Contador de resoluções (com repetição) para dado tabuleiro. Começa com zero */
} GameBoard;



/* Assinatura das funções (implementação após a "main") */
Solution makeSolution(int, int, char);
bool isValidPosition(int, int, GameBoard &);
void solve(int, int, int, GameBoard &);



/* == MAIN == */

int main(int argc, char **argv){
	set<Solution>::iterator it; /* C++ STL: para iterar nas soluções do dado tabuleiro */
	GameBoard board; /* Tabuleiro que será solucionado */
	int i, j, n; /* O "n" é o contador de cédulas não vazias (pedra, tesoura ou papel), necessário no algoritmo */

	/* Inicializa parâmetros do tabuleiro */
	board.solutions = set<Solution>();
	board.solutionCounter = 0;

	/* Lê R e C (números de linhas e colunas do tabuleiro) */
	cin >> board.r >> board.c;

	/* Aloca memória necessária e faz a leitura das cédulas */
	board.puzzle = (char **) malloc(sizeof(char *) * board.r);
	for(i = n = 0; i < board.r; i++) {
		board.puzzle[i] = (char *) malloc(sizeof(char) * board.c);
		for(j = 0; j < board.c; j++) {
			cin >> board.puzzle[i][j];
			board.puzzle[i][j] -= '0'; /* Para obter o valor inteiro, e não o da tabela ASCII */
			if(board.puzzle[i][j] != nothing)
				n++; /* Se for não vazio (pedra, tesoura ou papel), incrementa o contador */
		}
	}

	/* Chama a função backtracking e resolve o tabuleiro */
	solve(0, 0, n, board);

	/* Imprimir resultados encontrados na saída padrão */
	cout << board.solutionCounter << endl << board.solutions.size() << endl;
	for(it = board.solutions.begin(); it != board.solutions.end(); it++) {
		cout << ((*it).x + 1) << " " << ((*it).y + 1) << " " << ((int) (*it).type) << endl;
	}

	/* Desalocar memória alocada anteriormente */
	for(i = n = 0; i < board.r; i++) {
		free(board.puzzle[i]);
	}
	free(board.puzzle);

	return EXIT_SUCCESS;
}



/* == IMPLEMENTAÇÕES DAS FUNÇÕES == */

/* Função usada para criar uma estrutura que representa uma resolução */
Solution makeSolution(int x, int y, char type) {
	Solution auxObj;
	auxObj.x = x;
	auxObj.y = y;
	auxObj.type = type;
	return auxObj;
}

/* Função usada para verificar se dados i e j são válidos dentro da matriz que representa o tabuleiro */
bool isValidPosition(int i, int j, GameBoard &board) {
	return (i >= 0 && i < board.r && j >= 0 && j < board.c);
}

/* Função backtracking responsável por encontrar todas as soluções para o tabuleiro */
void solve(int x, int y, int n, GameBoard &board) {
	int i, j, k;

	if(n <= 1) { /* Se tem apenas uma cédula não vazia (pedra, tesoura ou papel), então chegou numa resolução */
		/* Condição de parada/fim: tem uma "vencedora" */
		board.solutions.insert(makeSolution(x, y, board.puzzle[x][y])); /* Insere a resolução no conjunto de resoluções (repetições são ignoradas) */
		board.solutionCounter++; /* Aumenta o contador de resoluções */
		return;
	}

	for(i = 0; i < board.r; i++){
		for(j = 0; j < board.c; j++){
			/* Para cada cédula do tabuleiro */
			switch(board.puzzle[i][j]) {
				case rock: /* Se for pedra, só vai mover se tiver tesouras adjacentes */
					for(k = 0; k < 4; k++) { /* Para cada movimento possível da cédula... */
						if(!isValidPosition(i + moves[k][0], j + moves[k][1], board)) /* Verificar se é válido */
							continue;
						if(board.puzzle[i + moves[k][0]][j + moves[k][1]] == scissors) { /* E verificar se tem tesoura */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = rock; /* Tem tesoura. Virou pedra agora porque pedra ganha! */
							board.puzzle[i][j] = nothing;
							solve(i + moves[k][0], j + moves[k][1], n - 1, board); /* Chamar recursão */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = scissors; /* Quando volta da recursão, tem que voltar ao estado original */
							board.puzzle[i][j] = rock;
						}
					}
				break;
				case scissors: /* Se for tesoura, só vai mover se tiver papeis adjacentes */
					for(k = 0; k < 4; k++) { /* Para cada movimento possível da cédula... */
						if(!isValidPosition(i + moves[k][0], j + moves[k][1], board)) /* Verificar se é válido */
							continue;
						if(board.puzzle[i + moves[k][0]][j + moves[k][1]] == paper) { /* E verificar se tem papel */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = scissors; /* Tem papel. Virou tesoura agora porque tesoura ganha! */
							board.puzzle[i][j] = nothing;
							solve(i + moves[k][0], j + moves[k][1], n - 1, board); /* Chamar recursão */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = paper; /* Quando volta da recursão, tem que voltar ao estado original */
							board.puzzle[i][j] = scissors;
						}
					}
				break;
				case paper: /* Se for papel, só vai mover se tiver pedras adjacentes */
					for(k = 0; k < 4; k++) { /* Para cada movimento possível da cédula... */
						if(!isValidPosition(i + moves[k][0], j + moves[k][1], board)) /* Verificar se é válido */
							continue;
						if(board.puzzle[i + moves[k][0]][j + moves[k][1]] == rock) {  /* E verificar se tem pedra */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = paper; /* Tem pedra. Virou papel agora porque papel ganha! */
							board.puzzle[i][j] = nothing;
							solve(i + moves[k][0], j + moves[k][1], n - 1, board); /* Chamar recursão */
							board.puzzle[i + moves[k][0]][j + moves[k][1]] = rock; /* Quando volta da recursão, tem que voltar ao estado original */
							board.puzzle[i][j] = paper;
						}
					}
				break;
			}
		}
	}

}


