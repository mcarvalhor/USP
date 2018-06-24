


#
#	~       Trabalho Prático: Parte 2       ~
#
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
## make clean   -> Remover arquivos *.o gerados, arquivos binários *.dat e o próprio executável do programa



all: programaTrab2

noobj: programaTrab2
	rm objects/*.o

programaTrab2: objects/main.o
	gcc -o programaTrab2 objects/*.o -Wall -pedantic -ansi

objects/main.o: main.c objects/funcionalidades.o headers/funcionalidades.h
	gcc -c -o objects/main.o main.c -I headers -Wall -pedantic -ansi

objects/funcionalidades.o: lib/funcionalidades.c objects/arvore-b.o headers/arvore-b.h objects/cabecalho.o headers/cabecalho.h
	gcc -c -o objects/funcionalidades.o lib/funcionalidades.c -I headers -Wall -pedantic -ansi

objects/arvore-b.o: lib/arvore-b.c objects/cabecalho.o headers/cabecalho.h
	gcc -c -o objects/arvore-b.o lib/arvore-b.c -I headers -Wall -pedantic -ansi

objects/cabecalho.o: lib/cabecalho.c headers/cabecalho.h
	gcc -c -o objects/cabecalho.o lib/cabecalho.c -I headers -Wall -pedantic -ansi

run: programaTrab2
	./programaTrab2

clean:
	rm programaTrab2 *.dat buffer-info.text objects/*.o
