
# ECE 361 - Homework 7
## AI Usage Documentation

### AI Tool Used

ChatGPT (OpenAI)

### Purpose of AI Assistance

ChatGPT was used to assist with implementing, testing,
debugging, and documenting Homework 7.

The assistance included explanations of C programming
concepts, code development, testing procedures, and
Git version control.

### Part 1: Integer Stack ADT

ChatGPT assisted with developing istack.h and istack.c.

The implementation uses:
- An opaque stack structure.
- Dynamic memory allocation with malloc.
- Automatic capacity doubling with realloc.
- Memory cleanup with free.
- Error handling for invalid arguments.
- Stack operations including push, pop, and peek.

Additional explanatory comments were added throughout
the implementation to support understanding and review.

The code was compiled using GCC with warning flags,
and a temporary test program verified basic behavior.

### Part 2: Bracket Checker

ChatGPT assisted with implementing balance.c.

The program uses the integer stack to check matching
parentheses, square brackets, and curly braces.

The implementation includes:
- Character-by-character input processing.
- Line and column tracking.
- Bracket matching and error reporting.
- Maximum nesting depth tracking.
- Proper stack cleanup.

The program was compiled and tested with balanced
and unbalanced inputs.

### Part 3: Automated Testing

ChatGPT assisted with developing:
- tests/test_istack.c
- Six bracket-checker input files.
- Expected output and exit status files.
- regress.sh
- Makefile

The stack tests cover capacity growth, LIFO behavior,
NULL arguments, array copying, and memory cleanup.

The regression tests compare program output and
exit status against expected results.

All 10 stack tests and 6 regression tests passed.

### Part 4: Memory Error Experiments

ChatGPT provided guidance for intentionally creating
three memory errors:

1. Destroying the same stack twice.
2. Pushing onto a stack after destroying it.
3. Omitting istack_destroy in balance.c.

AddressSanitizer and LeakSanitizer detected the
intentional memory errors.

The errors were documented in README.md.

Each intentional bug was removed after testing.

### Testing and Verification

The following commands were used:

    make
    make test
    make memcheck
    make clean

The final results were:

- Stack tests: 10 passed, 0 failed.
- Regression tests: 6 passed, 0 failed.
- No sanitizer errors in the final memory check.
- No compiler warnings in the verified builds.

### Corrections and Review

The source files were compiled and tested in Ubuntu
under Windows Subsystem for Linux (WSL).

Test results and sanitizer reports were reviewed.

Intentional memory errors introduced for Part 4
were restored to their original working versions.

ChatGPT also assisted with organizing the homework
files inside the existing ECE 361 Git repository.

### Final Statement

ChatGPT was used as a programming and learning
assistant throughout Homework 7.

The generated code and suggested commands were
entered, compiled, and tested in the local development
environment. Results were checked against the
assignment requirements.

The final submitted source code does not contain
the intentionally introduced memory errors.
