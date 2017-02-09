all: output
output: main.o
	gcc main.o -o lab
main.o: main.c
	gcc -c -Wall -pedantic -O2 -fno-tree-vectorize main.c
vectorize:
	gcc -c -Wall -pedantic -O2 -ftree-vectorize main.c
clean:
	rm -f *.o *.s lab
