CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

OBJS = main.o iom361_r4.o sensor.o fsm.o store.o aux.o report.o

.PHONY: all test clean

all: sauna

sauna: $(OBJS)
	$(CC) $(CFLAGS) -o sauna $(OBJS) $(LDLIBS)

main.o: main.c iom361_r4.h sensor.h fsm.h store.h aux.h report.h

iom361_r4.o: iom361_r4.c iom361_r4.h
sensor.o: sensor.c sensor.h
fsm.o: fsm.c fsm.h
store.o: store.c store.h sensor.h
aux.o: aux.c aux.h
report.o: report.c report.h

test_store_week4: test_store_week4.c store.c store.h sensor.h
	$(CC) $(CFLAGS) test_store_week4.c store.c -o test_store_week4

test_fsm: tests/test_fsm.c fsm.c fsm.h
	$(CC) $(CFLAGS) tests/test_fsm.c fsm.c -o test_fsm

test: sauna test_store_week4 test_fsm
	./sauna
	./test_store_week4
	./test_fsm

clean:
	rm -f sauna $(OBJS) test_store_week4 test_fsm
