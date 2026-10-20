HW03 Pointer Library
Author: Alex Freeman

Files
ptr.h - function declarations and documentation
ptr.c - implementations of pointer-based functions
tests/test_ptr.c - unit tests
Makefile - build and test commands
Building
To compile the library and test program:

make
To run all tests:
make test
To remove generated files:
make clean

Functions

The library implements:

swap_float() - swaps two float values
minmax() - finds minimum and maximum values in an array
find_above() - finds the first array element above a threshold
reverse() - reverses an array in place

All array traversal in ptr.c is performed using pointer arithmetic.

