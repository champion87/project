CC = gcc
FLAGS = -ansi -Wall -Wextra -Werror -pedantic-errors
LIBS = -lm

TARGET = symnmf

SRC = symnmf.c matrix_utils.c symnmf_algo.c

OBJ = symnmf.o matrix_utils.o symnmf_algo.o

all: symnmf

# Compile object files
symnmf.o: symnmf.c
	$(CC) $(CFLAGS) -c symnmf.c

matrix_utils.o: matrix_utils.c
	$(CC) $(CFLAGS) -c matrix_utils.c

symnmf_algo.o: symnmf_algo.c
	$(CC) $(CFLAGS) -c symnmf_algo.c

# Link object files into the final executable
symnmf: symnmf.o matrix_utils.o symnmf_algo.o
	$(CC) symnmf.o matrix_utils.o symnmf_algo.o -o symnmf $(LIBS)

# Clean up compiled files
clean:
	rm -f symnmf symnmf.o matrix_utils.o symnmf_algo.o