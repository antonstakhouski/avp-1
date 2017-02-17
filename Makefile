CC = gcc
CFLAGS = -c -std=c11 -save-temps -fverbose-asm -Wall -masm=intel -O2
OUTPUT_NAME = lab
BUILD = $(CC) -lrt main.o -o $(OUTPUT_NAME)

all: no-vect
no-vect: main.o-no-vect
	$(BUILD)
vect: main.o-vect
	$(BUILD)
main.o-no-vect: main.c
	$(CC) $(CFLAGS) -fno-tree-vectorize -mno-sse2 main.c
main.o-vect: main.c
	$(CC) $(CFLAGS) -ftree-vectorize -msse2 main.c
clean:
	rm -f *.o *.s *.i lab
