# Homework 4 — Tally ADT

## Overview

This homework implements an Abstract Data Type (ADT) in C called a tally. The program counts how many times different names appear and stores those names in alphabetical order.

The project is separated into modules so that each file has a specific responsibility.

### Main Modules

- **tally.h:** The public interface. It contains the constants, the opaque `tally_t` declaration, and the function prototypes used by other files.
- **tally.c:** The implementation of the tally ADT. It contains the private structure, the static array of tallies, and the functions for adding, counting, and printing names.
- **events.c:** The main program. It reads lines from standard input and uses the functions declared in `tally.h` to process event names.
- **tests/test_tally.c:** The unit test program. It checks the tally functions, including normal operation, invalid inputs, and maximum capacity.
- **Makefile:** Contains the rules for compiling the program, running tests, and removing compiled files.

## Building the Program

To build the program, open Ubuntu, navigate to the `hw04` directory, and run:

```bash
make
```

The Makefile uses GCC with C11 and warning flags to compile `events.c` and `tally.c`, then links the object files into the `events` executable.

## Running the Program

The `events` program reads event names from standard input.

For example:

```bash
./events < data/door.txt
```

The program processes the input one line at a time using `fgets()`.

It ignores blank lines and lines beginning with `#`. Other lines are passed to `tally_add()`, which either stores a new name or increases the count of an existing name.

Once all the input has been processed, `tally_print()` displays the names and counts in alphabetical order.

The program finishes by printing the number of different names and how many lines were rejected by `tally_add()`.

## Running the Tests

To build and execute the unit tests:

```bash
make test
```

The test program checks all six tally functions, including:

- Adding names and increasing existing counts.
- Looking up names that have never been added.
- Rejecting empty or excessively long names.
- Handling NULL arguments.
- Enforcing the maximum of 16 different names.
- Enforcing the maximum of four simultaneous tallies.
- Reusing tally slots after destruction.
- Keeping different tallies independent.
- Printing names in alphabetical order.

Each test prints PASS or FAIL, followed by a summary.

The test program returns a nonzero exit status if a test fails.

## Cleaning the Program

To remove the compiled executable and object files:

```bash
make clean
```

This allows the program to be rebuilt from its source files.

## Part 3 — What the Opaque Type Prevents

### Opaque Type Declaration

In `tally.h`, the tally type is declared as:

```c
typedef struct tally tally_t;
```

This declaration tells other files that the type exists, but does not reveal its internal members.

The complete structure is defined privately in `tally.c`:

```c
struct tally {
    struct entry entries[TALLY_NAMES];
    int size;
    int in_use;
};
```

Other source files can use pointers to `tally_t`, but they cannot directly access the fields inside the structure.

### Compiler Error Demonstration

I created a temporary C file called `opaque_demo.c` that included only `tally.h` and attempted to access a private member directly:

```c
tally_t *t = tally_create();
int n = t->size;
```

When I compiled the file, GCC produced the following error:

```text
opaque_demo.c: In function 'main':
opaque_demo.c:7:14: error: invalid use of incomplete typedef 'tally_t' {aka 'struct tally'}
```

This compiler error occurs because `tally.h` contains an incomplete structure declaration. The compiler knows that `tally_t` exists, but cannot determine which fields the structure contains.

### Why Opaque Types Are Useful

The opaque type prevents other source files from directly accessing or changing the internal variables of the tally. In `tally.h`, the structure is only declared, while its complete definition is hidden in `tally.c`. Because of this, code outside `tally.c` cannot use `t->size` or directly modify the stored counts. Instead, it must use functions like `tally_add()` and `tally_count()`. This is useful because it protects the tally's internal data from accidental changes and allows us to change how the tally works internally without changing programs that use it.

### Removing the Invalid Code

After observing the compiler error, I removed the temporary demonstration file. The submitted program contains only valid C code and can compile normally.

The error shown above is included for documentation purposes.
