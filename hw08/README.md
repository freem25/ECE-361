
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
