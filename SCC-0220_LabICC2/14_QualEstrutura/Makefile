
#
#	~  Qual é a estrutura?  ~
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

objects/main.o: main.c objects/stack.o headers/stack.h objects/queue.o headers/queue.h objects/heap.o headers/heap.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/stack.o: lib/stack.c headers/stack.h
	gcc -c -o objects/stack.o lib/stack.c -Wall -I headers

objects/queue.o: lib/queue.c headers/queue.h
	gcc -c -o objects/queue.o lib/queue.c -Wall -I headers

objects/heap.o: lib/heap.c headers/heap.h
	gcc -c -o objects/heap.o lib/heap.c -Wall -I headers

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
