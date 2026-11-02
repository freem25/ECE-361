
# ECE 361 - Homework 5
## Combination Lock Finite State Machine

### Overview

This program implements a combination lock using a
table-driven finite state machine (FSM) written in C.

The correct combination is 3-1-4.

Instead of using multiple if statements to determine
state transitions, the program uses a two-dimensional
transition table indexed by the current state and event.

Each table entry specifies:
- The next state
- The action to perform

### States

The FSM contains four states:

- LOCKED: Waiting for the first correct digit (3).
- GOT1: First digit (3) entered; waiting for 1.
- GOT2: First two digits (3, 1) entered; waiting for 4.
- OPEN: Correct combination entered; door is unlocked.

### Events

The FSM recognizes four events:

- E_RIGHT: Correct next digit was entered.
- E_WRONG: Incorrect digit was entered.
- E_CLOSE: Door closing event ('c').
- E_TIMEOUT: Timer expiration event ('t').

### Actions

- A_UNDEFINED: Indicates an undefined transition entry.
- A_NONE: No action.
- A_UNLOCK: Unlock the door.
- A_LOCK: Lock the door.
- A_BEEP: Beep when an incorrect digit is entered.

### State Diagram

The main path through the FSM is:

```text
                RIGHT(3) / NONE
    [LOCKED] ----------------------> [GOT1]
                                        |
                                        | RIGHT(1) / NONE
                                        |
                                        v
                                      [GOT2]
                                        |
                                        | RIGHT(4) / UNLOCK
                                        |
                                        v
                                      [OPEN]
                                        |
                           CLOSE or TIMEOUT / LOCK
                                        |
                                        v
                                    [LOCKED]
```

Additional transitions:

```text
LOCKED:
  WRONG / BEEP     -> LOCKED
  CLOSE / NONE     -> LOCKED
  TIMEOUT / NONE   -> LOCKED

GOT1:
  WRONG / BEEP     -> LOCKED
  CLOSE / NONE     -> GOT1
  TIMEOUT / NONE   -> LOCKED

GOT2:
  WRONG / BEEP     -> LOCKED
  CLOSE / NONE     -> GOT2
  TIMEOUT / NONE   -> LOCKED

OPEN:
  RIGHT / NONE     -> OPEN
  WRONG / NONE     -> OPEN
```

A wrong digit resets the combination and causes a beep.
A timeout while entering the code resets the FSM silently.
Closing the door while entering the code changes nothing.
While OPEN, digits have no effect.
Closing or timing out while OPEN locks the door.

### Transition Table

Each entry contains NEXT STATE / ACTION.

| Current State | E_RIGHT | E_WRONG | E_CLOSE | E_TIMEOUT |
|---|---|---|---|---|
| LOCKED | GOT1 / NONE | LOCKED / BEEP | LOCKED / NONE | LOCKED / NONE |
| GOT1 | GOT2 / NONE | LOCKED / BEEP | GOT1 / NONE | LOCKED / NONE |
| GOT2 | OPEN / UNLOCK | LOCKED / BEEP | GOT2 / NONE | LOCKED / NONE |
| OPEN | OPEN / NONE | OPEN / NONE | LOCKED / LOCK | LOCKED / LOCK |

The transition table has 4 states and 4 events,
giving a total of 16 entries.

### Program Files

- lock.h: State, event, action, and transition definitions.
- lock.c: Transition table and FSM functions.
- main.c: Reads input and prints state transitions.
- tests/test_lock.c: Automated tests for the FSM.
- data/codes.txt: Sample input for the lock.
- Makefile: Builds, tests, and cleans the program.

### Building the Program

From the hw05 directory, run:

```bash
make
```

This compiles the C files and creates the executable
named lock.

### Running the Program

To run using the sample input file:

```bash
./lock < data/codes.txt
```

The sample input contains:

```text
314c
```

Expected output:

```text
LOCKED 3 RIGHT -> GOT1 NONE
GOT1 1 RIGHT -> GOT2 NONE
GOT2 4 RIGHT -> OPEN UNLOCK
OPEN c CLOSE -> LOCKED LOCK
final: LOCKED
```

The program can also read from standard input.

For example:

```bash
printf '314\n' | ./lock
```

The character 'c' represents closing the door.
The character 't' represents a timer expiration.

The program processes timer events supplied as input;
it does not measure five seconds of real elapsed time.

Spaces, newlines, and unrecognized characters are ignored.

### Running the Tests

To compile and run the automated tests:

```bash
make test
```

The tests verify:
- All 16 state/event transition table entries.
- Correct and incorrect digit recognition.
- Door closing events.
- Timer expiration events.
- Invalid character handling.
- Complete input sequences.

The required tested sequences are:

- 314: Unlocks the door.
- 3314: Remains locked.
- 31t4: Remains locked after timeout.
- 314c: Unlocks and then locks again.

The test program displays PASS or FAIL for each test
and prints a summary.

A nonzero exit status indicates that at least one
test has failed.

### Cleaning Generated Files

To remove compiled object files and executables:

```bash
make clean
```

This leaves source files, tests, and documentation intact.
