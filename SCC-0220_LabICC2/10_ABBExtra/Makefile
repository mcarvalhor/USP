
#
#      ==         ABB         ==
#
#      Matheus Carvalho Raimundo
#      mcarvalhor@usp.br
#       _______ _______ _______
#      |   |   |               \
#      |   |   |      \    |___|
#      |   |    \      |   |
#      |_______ _______|___|
#
#

## make all	-> Apenas compilar
## make noobj	-> Compilar e remover arquivos *.o gerados
## make run	-> Executar programa compilado
## make clear	-> Remover arquivos *.o gerados e o próprio executável do programa


all: Prog

noobj: all
	rm *.o

Prog: Main.o
	gcc -o Prog *.o -Wall

Main.o: Main.c TreeBinSearch.o TreeBinSearch.h
	gcc -c -o Main.o Main.c -Wall -I.

TreeBinSearch.o: TreeBinSearch.c TreeBinSearch.h
	gcc -c -o TreeBinSearch.o TreeBinSearch.c -Wall

run: Prog
	./Prog

clean:
	rm Prog *.o

