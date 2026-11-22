
# ECE 361 - Homework 8

## Binary Search Tree ADT

This homework adapts the original BinarySearchTree.c
program into a reusable binary search tree ADT.

## Part 2: Original Search Bug

### Test That Triggered the Crash

Searching for key 42 in a newly created empty tree:

    bst_contains(t, 42, &visits);

### Memory Check

Command:

    make memcheck

First diagnostic line:

    bst.c:140:19: runtime error: member access within null pointer of type 'const struct node'

### Cause

The original search function accesses current->data
without first checking whether current is NULL.

The original program never noticed this problem because
its main function inserted seven keys before searching,
so it never attempted to search an empty tree.


## Building and Testing

### Build

Compile the BST test program:

    make

### Run Tests

Run all BST unit tests:

    make test

The tests cover:
- Searching an empty tree
- Inserting the original seven keys
- Checking tree size and height
- Searching for existing and missing keys
- Counting search visits
- Inserting duplicate keys
- Inserting keys in increasing order
- Destroying a full tree
- Handling NULL tree arguments

All tests passed with 0 failures.

### Memory Check

Run the tests with sanitizers enabled:

    make memcheck

The original search bug was reproduced using
AddressSanitizer and UndefinedBehaviorSanitizer.

After correcting bst_contains(), the tests
passed without sanitizer errors.

### Cleanup

Remove generated executables:

    make clean


## Part 3: What Order Does to a Tree

The treestats program reads integers from standard input,
inserts them into a binary search tree, and reports the
number of keys and the resulting tree height.

### Increasing Order

Command:

    seq 1 1023 | ./treestats

Output:

    n 1023, height 1023

### Shuffled Order

Command:

    seq 1 1023 | sort -R | ./treestats

Output:

    n 1023, height 25

### Explanation

Inserting keys in increasing order creates an unbalanced
tree with a height of 1023, while inserting shuffled keys
produced a much shorter tree with a height of 25.
A taller tree can require more node visits during a search,
whereas a shorter tree generally allows searches to finish
with fewer visits.

### Running treestats

Build the program:

    make

Run with an input file:

    ./treestats < input.txt

The program prints the tree size and height, reports
an empty tree when no integers are provided, and returns
an error for invalid input.
