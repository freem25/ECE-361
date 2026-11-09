
# ECE 361 - Homework 6
# Build and test the scenario replay program.

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -pedantic -I.
LDLIBS = -lm

# Object files shared by replay and the test program.
COMMON_OBJS = sensor.o panel.o scenario.o iom361_r4.o

.PHONY: all test clean

# Default target: build the replay program.
all: replay

# Build the main replay executable.
replay: main.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# Build the unit test executable.
test_hw06: tests/test_hw06.o $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# Header dependencies for each object file.
main.o: main.c sensor.h panel.h scenario.h iom361_r4.h

sensor.o: sensor.c sensor.h iom361_r4.h

panel.o: panel.c panel.h iom361_r4.h

scenario.o: scenario.c scenario.h

iom361_r4.o: iom361_r4.c iom361_r4.h

tests/test_hw06.o: tests/test_hw06.c sensor.h panel.h scenario.h iom361_r4.h

# Compile C source files into object files.
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# Run unit tests followed by regression tests.
test: replay test_hw06
	./test_hw06
	./regress.sh


# Remove generated files.
clean:
	rm -f replay test_hw06 *.o tests/*.o
