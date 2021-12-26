
programaTrab: all

all: objects/main.o
	gcc -g objects/*.o -o programaTrab

objects/main.o: main.c headers/funcionalidades.h headers/escreverTela.h objects/funcionalidades.o objects/escreverTela.o
	gcc -g -c main.c -I headers -o objects/main.o

objects/funcionalidades.o: lib/funcionalidades.c headers/bindex.h headers/index.h headers/register.h headers/buffer.h headers/merge_sort.h headers/vector_builder.h headers/consts.h headers/escreverTela.h objects/bindex.o objects/index.o objects/register.o objects/buffer.o objects/merge_sort.o objects/vector_builder.o objects/escreverTela.o
	gcc -g -c lib/funcionalidades.c -I headers -o objects/funcionalidades.o

objects/bindex.o: lib/bindex.c headers/bindex.h headers/register.h headers/buffer.h headers/consts.h objects/register.o objects/buffer.o
	gcc -g -c lib/bindex.c -I headers -o objects/bindex.o

objects/index.o: lib/index.c headers/index.h headers/register.h headers/buffer.h headers/consts.h objects/register.o objects/buffer.o
	gcc -g -c lib/index.c -I headers -o objects/index.o

objects/register.o: lib/register.c headers/register.h headers/buffer.h headers/consts.h objects/buffer.o
	gcc -g -c lib/register.c -I headers -o objects/register.o

objects/buffer.o: lib/buffer.c headers/buffer.h
	gcc -g -c lib/buffer.c -I headers -o objects/buffer.o

objects/merge_sort.o: lib/merge_sort.c
	gcc -g -c lib/merge_sort.c -I headers -o objects/merge_sort.o

objects/vector_builder.o: lib/vector_builder.c
	gcc -g -c lib/vector_builder.c -I headers -o objects/vector_builder.o

objects/escreverTela.o: lib/escreverTela.c
	gcc -g -c lib/escreverTela.c -I headers -o objects/escreverTela.o

barra-r:
	gcc -g barra-r.c -o barra-r

run: programaTrab
	./programaTrab

clean:
	rm objects/*.o programaTrab barra-r *.bin *.index *.btree *.in *.out
