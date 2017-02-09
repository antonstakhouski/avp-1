CC = gcc
CFLAGS = -c -save-temps -fverbose-asm -Wall -masm=intel -pedantic -O2
OUTPUT_NAME = lab
BUILD = $(CC) main.o -o $(OUTPUT_NAME)

all: no-vect
no-vect: main.o-no-vect
	$(BUILD)
vect: main.o-vect
	$(BUILD)
main.o-no-vect: main.c
	$(CC) $(CFLAGS) -fno-tree-vectorize main.c
main.o-vect: main.c
	$(CC) $(CFLAGS) -ftree-vectorize main.c
clean:
	rm -f *.o *.s lab
