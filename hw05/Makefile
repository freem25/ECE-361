
# ECE 361 - Homework 5
# Makefile for the combination lock finite state machine.

# Compiler used to build our C programs.
CC = gcc

# Compile using C11 and enable compiler warnings.
CFLAGS = -std=c11 -Wall -Wextra -pedantic

# These targets represent commands, not output files.
.PHONY: all test clean

# Default target: running "make" builds the lock program.
all: lock

# Link the object files into the lock executable.
lock: main.o lock.o
	$(CC) $(CFLAGS) main.o lock.o -o lock

# Compile main.c into an object file.
# Rebuild if either main.c or lock.h changes.
main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c -o main.o

# Compile lock.c into an object file.
# Rebuild if either lock.c or lock.h changes.
lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c -o lock.o

# Compile the test source into an object file.
# The test includes the lock.h header.
tests/test_lock.o: tests/test_lock.c lock.h
	$(CC) $(CFLAGS) -c tests/test_lock.c -o tests/test_lock.o

# Link the test object and lock implementation.
tests/test_lock: tests/test_lock.o lock.o
	$(CC) $(CFLAGS) tests/test_lock.o lock.o -o tests/test_lock

# Build and run our official test suite.
test: tests/test_lock
	./tests/test_lock

# Remove generated executables and object files.
# Keep all source code, headers, and documentation.
clean:
	rm -f lock main.o lock.o tests/test_lock.o tests/test_lock test_lock
