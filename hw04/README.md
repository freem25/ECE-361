# Homework 4 — Tally ADT

## Building the program

This homework implements a tally abstract data type in C. The tally counts how many times different names appear and keeps the names in alphabetical order.

To build the program, open Ubuntu, navigate to the `hw04` directory, and run:

```bash
make
```

This builds the `events` program using GCC with C11 and warning flags.

## Running the program

The `events` program reads event names from standard input. For example:

```bash
./events < door.txt
```

It ignores blank lines and lines starting with `#`, counts valid names, and prints the results alphabetically, followed by a summary showing the number of different names and rejected lines.

## Running the tests

To build and run the unit tests:

```bash
make test
```

The tests check counting, invalid names, NULL arguments, maximum capacities, independent tallies, creation and destruction, and alphabetical output.

To remove the compiled program and object files:

```bash
make clean
```

## What the opaque type prevents

I created a temporary C file called `opaque_demo.c` that included only `tally.h` and attempted to access a member of the tally structure directly:

```c
tally_t *t = tally_create();
int n = t->size;
```

When I compiled the file, GCC produced the following error:

```text
opaque_demo.c: In function 'main':
opaque_demo.c:7:14: error: invalid use of incomplete typedef 'tally_t' {aka 'struct tally'}
```

**Explanation:** The opaque type prevents other source files from directly accessing or changing the internal variables of the tally. In `tally.h`, the structure is only declared, while its complete definition is hidden in `tally.c`. Because of this, code outside `tally.c` cannot use `t->size` or directly modify the stored counts. Instead, it must use functions like `tally_add()` and `tally_count()`. This is useful because it protects the tally's internal data from accidental changes and allows us to change how the tally works internally without changing programs that use it.

After observing the compiler error, I removed the temporary test code so the submitted program compiles successfully.
