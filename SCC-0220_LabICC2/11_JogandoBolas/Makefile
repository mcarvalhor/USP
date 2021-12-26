
#
#	~    JOGANDO BOLAS      ~
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
	gcc -o Prog objects/*.o -Wall -lm

objects/main.o: main.c objects/SortedHeap.o headers/SortedHeap.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/SortedHeap.o: lib/SortedHeap.c headers/SortedHeap.h
	gcc -c -o objects/SortedHeap.o lib/SortedHeap.c -Wall -I headers

run: Prog
	./Prog

clean:
	rm Prog objects/*.o