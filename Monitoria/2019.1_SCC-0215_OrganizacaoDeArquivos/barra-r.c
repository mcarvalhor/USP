#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv) {
	char aux;

	while((aux = getc(stdin)) != EOF && aux != '\r');
	if(aux == EOF) {
		printf("Não tem barra R.\n");
	} else {
		printf("Tem barra R.\n");
	}

	return EXIT_SUCCESS;
}
