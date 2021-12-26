
#
#	~         FUTEBOL       ~
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

main.o: main.c soccer.o soccer.h
	gcc -c -o main.o main.c -Wall

soccer.o: soccer.c super_string.o super_string.h
	gcc -c -o soccer.o soccer.c -Wall -I.

super_string.o: super_string.c
	gcc -c -o super_string.o super_string.c -Wall

run: Prog
	./Prog

clear:
	rm Prog *.o


