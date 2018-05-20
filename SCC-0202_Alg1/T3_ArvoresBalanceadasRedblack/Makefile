
#
#	~      Rubro Negra      ~
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
	gcc -o Prog objects/*.o -ansi -pedantic -Wall

objects/main.o: main.c objects/redblacktree.o headers/redblacktree.h
	gcc -c -o objects/main.o main.c -ansi -pedantic -Wall -I headers

objects/redblacktree.o: lib/redblacktree.c headers/redblacktree.h
	gcc -c -o objects/redblacktree.o lib/redblacktree.c -ansi -pedantic -Wall -I headers

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
