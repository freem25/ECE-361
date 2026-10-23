
# Use GCC to compile our C programs.
CC = gcc

# Compile using C11 with warnings enabled.
# Werror makes warnings count as errors.
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic

# These are commands, not files to build.
.PHONY: all test clean

# Default target: build the events program.
all: events

# Link the object files into the events executable.
events: events.o tally.o
	$(CC) $(CFLAGS) events.o tally.o -o events

# Compile events.c into events.o.
# Rebuild when events.c or tally.h changes.
events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c -o events.o

# Compile tally.c into tally.o.
# Rebuild when tally.c or tally.h changes.
tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c -o tally.o

# Compile the test source into an object file.
tests/test_tally.o: tests/test_tally.c tally.h
	$(CC) $(CFLAGS) -c tests/test_tally.c -o tests/test_tally.o

# Link the test object file and tally implementation.
tests/test_tally: tests/test_tally.o tally.o
	$(CC) $(CFLAGS) tests/test_tally.o tally.o -o tests/test_tally

# Build and run the test program.
test: tests/test_tally
	./tests/test_tally

# Delete compiled programs and object files.
clean:
	rm -f events events.o tally.o tests/test_tally.o tests/test_tally
