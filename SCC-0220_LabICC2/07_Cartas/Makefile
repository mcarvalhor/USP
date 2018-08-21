
#
#	~         CARTAS        ~
#
#	Matheus Carvalho Raimundo
#	mcarvalhor@usp.br
#	 _______ _______ _______
#	|   |   |               \
#	|   |   |      \    |___|
#	|   |    \      |   |
#	|_______ _______|___|
#
#

## make all     -> Apenas compilar
## make noobj   -> Compilar e remover arquivos *.o gerados
## make run     -> Executar programa compilado
## make clean   -> Remover arquivos *.o gerados e o próprio executável do programa



all: Prog

noobj: Prog
	rm objects/*.o

Prog: objects/main.o
	gcc -o Prog objects/*.o -Wall

objects/main.o: main.c objects/cards.o headers/cards.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/cards.o: lib/cards.c objects/list.o headers/list.h
	gcc -c -o objects/cards.o lib/cards.c -Wall -I headers

objects/list.o: lib/list.c headers/list.h
	gcc -c -o objects/list.o lib/list.c -Wall -I headers

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
