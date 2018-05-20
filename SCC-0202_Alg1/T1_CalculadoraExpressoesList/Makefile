
#
#	~      Calculadora      ~
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

objects/main.o: main.c objects/calc.o headers/calc.h
	gcc -c -o objects/main.o main.c -Wall -ansi -pedantic -I headers

objects/calc.o: lib/calc.c objects/super_file.o headers/super_file.h objects/stack.o headers/stack.h objects/linkedlist.o headers/linkedlist.h
	gcc -c -o objects/calc.o lib/calc.c -Wall -ansi -pedantic -I headers

objects/linkedlist.o: lib/linkedlist.c headers/linkedlist.h
	gcc -c -o objects/linkedlist.o lib/linkedlist.c -Wall -ansi -pedantic -I headers

objects/stack.o: lib/stack.c headers/stack.h
	gcc -c -o objects/stack.o lib/stack.c -Wall -ansi -pedantic -I headers

objects/super_file.o: lib/super_file.c
	gcc -c -o objects/super_file.o lib/super_file.c -Wall -ansi -pedantic

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
