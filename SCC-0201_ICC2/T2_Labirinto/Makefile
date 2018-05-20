
#
#	~       LABIRINTO       ~
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
## make clear   -> Remover arquivos *.o gerados e o próprio executável do programa


all: Prog

noobj: all
	rm *.o

Prog: main.o
	gcc -o Prog *.o -Wall -lm

main.o: main.c labyrinth.o labyrinth.h
	gcc -c -o main.o main.c -Wall

labyrinth.o: labyrinth.c stack.o stack.h
	gcc -c -o labyrinth.o labyrinth.c -Wall -I.

stack.o: stack.c stack.h
	gcc -c -o stack.o stack.c -Wall

run: Prog
	./Prog

clear:
	rm Prog *.o


