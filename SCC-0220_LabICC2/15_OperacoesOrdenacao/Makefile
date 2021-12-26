
#
#	~    Op. Ordenacao      ~
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

objects/main.o: main.c objects/insertionsort.o headers/insertionsort.h objects/bubblesort.o headers/bubblesort.h objects/mergesort.o headers/mergesort.h objects/heapsort.o headers/heapsort.h objects/quicksort.o headers/quicksort.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/insertionsort.o: lib/insertionsort.c
	gcc -c -o objects/insertionsort.o lib/insertionsort.c -Wall

objects/bubblesort.o: lib/bubblesort.c objects/vectorutils.o headers/vectorutils.h
	gcc -c -o objects/bubblesort.o lib/bubblesort.c -Wall -I headers

objects/mergesort.o: lib/mergesort.c objects/vectorutils.o headers/vectorutils.h
	gcc -c -o objects/mergesort.o lib/mergesort.c -Wall -I headers

objects/heapsort.o: lib/heapsort.c objects/vectorutils.o headers/vectorutils.h
	gcc -c -o objects/heapsort.o lib/heapsort.c -Wall -I headers

objects/quicksort.o: lib/quicksort.c objects/vectorutils.o headers/vectorutils.h
	gcc -c -o objects/quicksort.o lib/quicksort.c -Wall -I headers

objects/vectorutils.o: lib/vectorutils.c
	gcc -c -o objects/vectorutils.o lib/vectorutils.c -Wall

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
