
#
#      ==   NÚMEROS IMENSOS   ==
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

Main.o: Main.c UnlimitedInteger.o UnlimitedInteger.h
	gcc -c -o Main.o Main.c -Wall

UnlimitedInteger.o: UnlimitedInteger.c UnlimitedInteger.h List.o List.h
	gcc -c -o UnlimitedInteger.o UnlimitedInteger.c -Wall

List.o: List.c List.h
	gcc -c -o List.o List.c -Wall

run: Prog
	./Prog

clear:
	rm Prog *.o

