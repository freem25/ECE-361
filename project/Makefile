CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

OBJS = main.o iom361_r4.o sensor.o scenario.o fsm.o store.o aux.o report.o bits.o sauna_status.o

.PHONY: all test clean

all: sauna

sauna: $(OBJS)
	$(CC) $(CFLAGS) -o sauna $(OBJS) $(LDLIBS)

main.o: main.c iom361_r4.h sensor.h scenario.h fsm.h store.h aux.h report.h sauna_status.h

iom361_r4.o: iom361_r4.c iom361_r4.h
sensor.o: sensor.c sensor.h
scenario.o: scenario.c scenario.h
bits.o: bits.c bits.h
sauna_status.o: sauna_status.c sauna_status.h bits.h fsm.h
fsm.o: fsm.c fsm.h
store.o: store.c store.h sensor.h
aux.o: aux.c aux.h
report.o: report.c report.h

test_store_week4: test_store_week4.c store.c store.h sensor.h
	$(CC) $(CFLAGS) test_store_week4.c store.c -o test_store_week4

test_fsm: tests/test_fsm.c fsm.c fsm.h
	$(CC) $(CFLAGS) tests/test_fsm.c fsm.c -o test_fsm

# Test the sauna status-word packing and unpacking.
test_sauna_status: tests/test_sauna_status.c sauna_status.c bits.c sauna_status.h bits.h fsm.h
	$(CC) $(CFLAGS) -I. tests/test_sauna_status.c sauna_status.c bits.c -o test_sauna_status




# Run the sauna scenario and all current unit tests.
test: sauna test_store_week4 test_fsm test_sauna_status
	./sauna scenarios/01_normal.txt > /dev/null
	./test_store_week4
	./test_fsm
	./test_sauna_status



clean:
	rm -f sauna $(OBJS) test_store_week4 test_fsm test_sauna_status
