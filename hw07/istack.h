
#ifndef ISTACK_H
#define ISTACK_H

#include <stdbool.h>
#include <stddef.h>

/* Initial capacity of the stack. */
#define ISTACK_START 8

/* Opaque stack type. Its structure is defined in istack.c. */
typedef struct istack istack_t;

/*
 * Creates an empty stack with room for ISTACK_START integers.
 * Takes no arguments.
 * Returns a pointer to the stack, or NULL if allocation fails.
 */
istack_t *istack_create(void);

/*
 * Frees all memory associated with the stack.
 * Takes a pointer to the stack; NULL is allowed.
 * Returns nothing.
 */
void istack_destroy(istack_t *s);

/*
 * Adds integer v to the top of stack s.
 * Doubles the capacity when the stack is full.
 * Returns 0 on success or -1 on failure.
 * On failure, the stack remains unchanged.
 */
int istack_push(istack_t *s, int v);

/*
 * Removes the top integer from stack s.
 * Stores it in *v if v is not NULL.
 * Returns 0 on success or -1 if s is NULL or empty.
 * On failure, *v remains unchanged.
 */
int istack_pop(istack_t *s, int *v);

/*
 * Reads the top integer without removing it.
 * Stores it in *v if v is not NULL.
 * Returns 0 on success or -1 if s is NULL or empty.
 * On failure, *v remains unchanged.
 */
int istack_peek(const istack_t *s, int *v);

/*
 * Returns the number of integers currently stored.
 * Returns 0 if s is NULL.
 */
size_t istack_count(const istack_t *s);

/*
 * Checks whether the stack is empty.
 * Returns true if empty or s is NULL; false otherwise.
 */
bool istack_is_empty(const istack_t *s);

/*
 * Returns the current capacity of the stack.
 * Returns 0 if s is NULL.
 */
size_t istack_capacity(const istack_t *s);

/*
 * Creates a new array containing the stack values,
 * ordered from bottom to top.
 * Writes the number of values into *n.
 * Returns NULL and sets *n to 0 for a NULL or empty
 * stack, or if allocation fails.
 * Assumes n is a valid pointer; a NULL n returns NULL.
 * The caller must free the returned array.
 */
int *istack_to_array(const istack_t *s, size_t *n);

#endif
