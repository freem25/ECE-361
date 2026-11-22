
# Compiler and warning settings.
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

# Sanitizers for detecting memory errors.
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

# Build both the tree statistics and test programs.
all: treestats test_bst

# Build the tree statistics program.
treestats: treestats.c bst.c bst.h
	$(CC) $(CFLAGS) treestats.c bst.c -o treestats

# Build the BST unit tests.
test_bst: bst.c bst.h tests/test_bst.c
	$(CC) $(CFLAGS) bst.c tests/test_bst.c -o test_bst

# Run the regular unit tests.
test: test_bst
	./test_bst

# Run tests with memory sanitizers enabled.
memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) bst.c tests/test_bst.c -o test_bst_memcheck
	./test_bst_memcheck

# Remove generated executable files.
clean:
	rm -f treestats test_bst test_bst_memcheck
