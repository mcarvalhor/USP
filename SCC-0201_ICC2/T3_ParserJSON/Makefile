
#
#	~      JSON Parser      ~
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

objects/main.o: main.c objects/json.o headers/json.h
	gcc -c -o objects/main.o main.c -Wall -I headers

objects/json.o: lib/json.c objects/super_file.o headers/super_file.h objects/stack.o headers/stack.h
	gcc -c -o objects/json.o lib/json.c -Wall -I headers

objects/stack.o: lib/stack.c headers/stack.h
	gcc -c -o objects/stack.o lib/stack.c -Wall -I headers

objects/super_file.o: lib/super_file.c
	gcc -c -o objects/super_file.o lib/super_file.c -Wall

run: Prog
	./Prog

clean:
	rm Prog objects/*.o
