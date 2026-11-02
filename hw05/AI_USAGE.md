# ECE 361 - Homework 5: AI Usage

## AI Tool Used

- **Tool:** ChatGPT (OpenAI)
- **Purpose:** Assistance with understanding finite state machines, designing the transition table, writing and explaining C code, developing tests, and preparing documentation.

## How AI Was Used

I used ChatGPT to help me understand and implement the combination-lock finite state machine for Homework 5.

Specifically, ChatGPT helped with:

1. Explaining the four states (LOCKED, GOT1, GOT2, OPEN) and how the combination 3-1-4 moves between them.
2. Designing the state diagram and the 16-entry transition table.
3. Developing `lock.h` and `lock.c`, including comments explaining the functions and transitions.
4. Explaining how `lock_step()` uses a table lookup and how `lock_event()` interprets input characters.
5. Creating `main.c` to process standard input and print state transitions.
6. Developing automated tests for transitions, events, invalid characters, and input sequences.
7. Creating the Makefile and README documentation.

## Verification and Corrections

I entered the code into files using nano in Ubuntu under WSL and compiled it with GCC using:

`gcc -std=c11 -Wall -Wextra -pedantic`

One limitation of the initial AI-assisted testing approach was that it checked only four example combinations. Those tests alone were not sufficient to verify all the required behavior.

I addressed this by expanding the official test suite to check all 16 transition-table entries, digit recognition, closing and timeout events, invalid characters, and the four required input sequences.

The final test suite ran **40 tests with 0 failures**.

I also verified that:

- `make` builds the combination-lock program without compiler warnings.
- `make test` builds and runs the automated tests.
- `make clean` removes generated files without deleting source code.
- `./lock < data/codes.txt` produces the expected state transitions.

## What I Learned

This assignment helped me understand how a finite state machine can be represented using a two-dimensional table rather than a long series of conditional statements.

I also gained practice with enumerations, structures, pointers used for output parameters, character-by-character input, automated testing, Makefiles, and GitHub development workflows.
