
# ECE 361 - Homework 7
## Dynamic Integer Stack and Bracket Checker

### Overview

Homework 7 implements a dynamically allocated integer stack
(istack ADT) and a bracket-checking program that uses the stack.

The stack starts with a capacity of 8 integers and automatically
doubles its capacity when full.

The bracket checker reads standard input and verifies that
parentheses (), square brackets [], and curly braces {} are
correctly matched.

## Files

- istack.h: Public interface for the integer stack ADT.
- istack.c: Stack implementation using malloc, realloc, and free.
- balance.c: Bracket checker using the integer stack.
- tests/test_istack.c: Automated stack unit tests.
- inputs/: Input files for bracket-checker testing.
- expected/: Expected output and exit status for each input.
- regress.sh: Automated bracket-checker regression tests.
- Makefile: Build, test, memory-check, and cleanup commands.
- AI_USAGE.md: Documentation of AI assistance.

## Build Instructions

From the hw07 directory, run:

    make

This compiles the bracket checker and creates the balance executable.

To run the bracket checker with an input file:

    ./balance < inputs/balanced.txt

Example output:

    balanced: 3 pairs, deepest 3

## Testing Instructions

Run all unit tests and regression tests:

    make test

This runs 10 integer-stack tests and 6 bracket-checker tests.

The stack tests verify:
- Creating and destroying a stack.
- Empty stack behavior.
- Last-in, first-out ordering.
- Capacity doubling from 8 to 16.
- 1000 pushes and pops.
- Independent array copies.
- NULL argument handling.
- Optional output pointers.
- Independent stacks.
- Destroying a nonempty stack.

The regression tests verify:
- Balanced brackets.
- Nesting deeper than 8.
- Mismatched brackets.
- Closing brackets with nothing open.
- Unclosed opening brackets.
- Empty input.

All 16 tests passed.

## Memory Checking

Run:

    make memcheck

This rebuilds the programs using AddressSanitizer and
UndefinedBehaviorSanitizer and runs the test suites.

The final memory checks passed with no sanitizer errors.

To remove compiled programs and object files:

    make clean

## Part 4: Intentional Memory Errors

Three memory bugs were intentionally introduced and tested
using AddressSanitizer and LeakSanitizer.

### Bug 1: Destroying the Same Stack Twice

The program intentionally called istack_destroy(s) twice.

Sanitizer result:

    ==975==ERROR: AddressSanitizer: heap-use-after-free on address 0x5030000003a0 at pc 0x5aa7b8b99307 bp 0x7ffe1af29390 sp 0x7ffe1af29380

The first call freed the stack's allocated memory.
The second call attempted to access the already-freed stack,
causing a heap-use-after-free error.

The extra call was removed and make memcheck passed afterward.

### Bug 2: Pushing After Destroying the Stack

The program intentionally called istack_push(s, 999)
after calling istack_destroy(s).

Sanitizer result:

    ==1088==ERROR: AddressSanitizer: heap-use-after-free on address 0x5030000003a8 at pc 0x62f62b8933c7 bp 0x7ffc21b2cda0 sp 0x7ffc21b2cd90

The push operation attempted to access the stack structure
after its memory had already been freed.

The invalid push was removed and make memcheck passed afterward.

### Bug 3: Missing istack_destroy()

The final istack_destroy(s) call was temporarily removed
from balance.c.

Sanitizer result:

    ==1185==ERROR: LeakSanitizer: detected memory leaks

The program exited without releasing the dynamically allocated
stack structure and its integer array.

LeakSanitizer reported 56 bytes leaked in 2 allocations:
24 bytes for the structure and 32 bytes for the integer array.

The missing istack_destroy(s) call was restored.
The final make memcheck passed with no memory errors.

## Final Verification

- make: PASS
- make test: PASS
- Stack unit tests: 10 passed, 0 failed
- Bracket regression tests: 6 passed, 0 failed
- make memcheck: PASS
- make clean: PASS

All intentional memory errors have been removed from the
submitted source code.
