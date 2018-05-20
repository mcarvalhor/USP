
#
#	~  Gerenc. de Processos ~
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

objects/main.o: main.c objects/processman.o headers/processman.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/processman.o: lib/processman.c
	gcc -c -o objects/processman.o lib/processman.c -Wall

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
