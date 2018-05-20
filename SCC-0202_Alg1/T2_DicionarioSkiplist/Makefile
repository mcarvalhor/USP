
#
#	~       Dicionário      ~
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
	gcc -o Prog objects/*.o -Wall -ansi -pedantic

objects/main.o: main.c objects/skiplist.o headers/skiplist.h
	gcc -c -o objects/main.o main.c -Wall -ansi -pedantic -I headers

objects/skiplist.o: lib/skiplist.c headers/skiplist.h
	gcc -c -o objects/skiplist.o lib/skiplist.c -Wall -ansi -pedantic -I headers

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
