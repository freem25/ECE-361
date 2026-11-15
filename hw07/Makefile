
# ECE 361 - Homework 7
# Makefile for the integer stack and bracket checker

# C compiler
CC = gcc

# Compilation flags
CFLAGS = -std=c11 -Wall -Wextra -pedantic -g

# Allow source files in tests/ to find istack.h
CPPFLAGS = -I.

# Memory sanitizer flags
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

# Default target: build the bracket checker
all: balance

# Build the bracket checker
balance: balance.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) balance.c istack.c -o balance

# Build the integer stack test program
tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CPPFLAGS) $(CFLAGS) $(LDFLAGS) tests/test_istack.c istack.c -o tests/test_istack

# Run the stack tests and bracket checker regression tests
test: balance tests/test_istack
	./tests/test_istack
	./regress.sh

# Rebuild with AddressSanitizer and UndefinedBehaviorSanitizer
# Run all tests with memory checking enabled
memcheck:
	$(MAKE) clean
	$(MAKE) CFLAGS="$(CFLAGS) $(SANFLAGS)" LDFLAGS="$(SANFLAGS)" test
	$(MAKE) clean

# Remove compiled executables and object files
clean:
	rm -f balance tests/test_istack *.o

# These are commands, not output files
.PHONY: all test memcheck clean
