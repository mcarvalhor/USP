#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include <vector>
#include <stack>
#include <queue>



/*
 *      ==    AlgAvançados   ==
 *
 *      Matheus Carvalho Raimundo (Nº 10369014)
 *       _______ _______ _______
 *      |   |   |               \
 *      |   |   |      \    |___|
 *      |   |    \      |   |
 *      |_______ _______|___|
 *
*/



using namespace std;

int main(int argc, char **argv) {
	int i, N, aux;

	queue<int> expectingCoachesOnB;
	stack<int> currentCoachesOnStation;

	while(scanf("%d", &N) == 1 && N != 0) { // Enquanto ainda existirem casos de teste...
		while(scanf("%d", &aux) == 1 && aux != 0) { // Enquanto ainda existirem casos de teste para este caso de teste...

			// Limpar fila e pilha.
			expectingCoachesOnB = queue<int>();
			currentCoachesOnStation = stack<int>();

			// Ler entrada.
			expectingCoachesOnB.push(aux);
			for(i = 1; i < N; i++) {
				scanf("%d", &aux);
				expectingCoachesOnB.push(aux);
			}

			// Processar entrada.
			for(i = 1; i <= N; i++) {
				currentCoachesOnStation.push(i);
				while(!currentCoachesOnStation.empty() && currentCoachesOnStation.top() == expectingCoachesOnB.front()) { // O que está na pilha é esperado no resultado final. Liberar coach então!
					expectingCoachesOnB.pop();
					currentCoachesOnStation.pop();
				}
			}

			// Processar o que está parado na estação (o resto).
			while(!currentCoachesOnStation.empty()) {
				if(expectingCoachesOnB.front() != currentCoachesOnStation.top()) { // Se forem diferentes, não dá certo... "break" impede que o código continue para o "pop".
					break;
				}
				expectingCoachesOnB.pop();
				currentCoachesOnStation.pop();
			}

			if(currentCoachesOnStation.empty()) { // Só dá certo se não sobrar nenhum coach na estação.
				printf("Yes\n");
			} else {
				printf("No\n");
			}
		}
		printf("\n");
	}
	
	return EXIT_SUCCESS;
}
