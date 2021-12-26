
CCF = -I headers -lm


run:
	./prog

all: prog


prog: bin/main.o bin/funcionalidades.o bin/register.o bin/escreverTela.o bin/indiceCidades.o bin/graph.o bin/buscaCampos.o
	gcc bin/*.o -o prog $(CCF)

bin/main.o: src/main.c headers/funcionalidades.h headers/consts.h
	gcc -c src/main.c -o bin/main.o $(CCF)

bin/funcionalidades.o: src/funcionalidades.c headers/funcionalidades.h headers/escreverTela.h headers/register.h headers/indiceCidades.h headers/graph.h headers/buscaCampos.h headers/consts.h
	gcc -c src/funcionalidades.c -o bin/funcionalidades.o $(CCF)

bin/buscaCampos.o: src/buscaCampos.c headers/buscaCampos.h headers/register.h headers/consts.h
	gcc -c src/buscaCampos.c -o bin/buscaCampos.o $(CCF)

bin/register.o: src/register.c headers/register.h headers/consts.h
	gcc -c src/register.c -o bin/register.o $(CCF)

bin/indiceCidades.o: src/indiceCidades.c headers/indiceCidades.h headers/consts.h
	gcc -c src/indiceCidades.c -o bin/indiceCidades.o $(CCF)

bin/graph.o: src/graph.c headers/graph.h headers/consts.h
	gcc -c src/graph.c -o bin/graph.o $(CCF)

bin/escreverTela.o: src/escreverTela.c headers/escreverTela.h
	gcc -c src/escreverTela.c -o bin/escreverTela.o $(CCF)


gerar:
	./prog < gerardados.in

clean:
	rm bin/*.o prog
