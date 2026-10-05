CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -g -I.

OBJS = bits.o status.o
TEST_OBJ = tests/test_bits.o

all: bits.o status.o tests/test_bits.o

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c bits.c -o bits.o

status.o: status.c status.h bits.h
	$(CC) $(CFLAGS) -c status.c -o status.o

tests/test_bits.o: tests/test_bits.c bits.h status.h
	$(CC) $(CFLAGS) -c tests/test_bits.c -o tests/test_bits.o

tests/test_bits: $(OBJS) $(TEST_OBJ)
	$(CC) $(CFLAGS) -o tests/test_bits $(OBJS) $(TEST_OBJ)

test: tests/test_bits
	./tests/test_bits

clean:
	rm -f bits.o status.o tests/test_bits.o tests/test_bits

.PHONY: all test clean

