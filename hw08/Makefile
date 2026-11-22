
# Compiler and warning settings.
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

# Sanitizers help detect invalid memory access.
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

# Build the BST test program.
all: test_bst

test_bst: bst.c bst.h tests/test_bst.c
	$(CC) $(CFLAGS) bst.c tests/test_bst.c -o test_bst

# Run the regular tests.
test: test_bst
	./test_bst

# Run tests with memory-error detection enabled.
memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) bst.c tests/test_bst.c -o test_bst_memcheck
	./test_bst_memcheck

# Remove generated executable files.
clean:
	rm -f test_bst test_bst_memcheck
