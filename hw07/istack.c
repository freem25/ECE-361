
#include "istack.h"

#include <stdint.h>
#include <stdlib.h>

/*
 * Private definition of the stack structure.
 * data:     Pointer to the dynamically allocated array of integers.
 * count:    Number of integers currently stored in the stack.
 * capacity: Maximum number of integers the array can currently hold.
 * This structure is defined here instead of istack.h
 * so that other files cannot directly access its members.
 */
struct istack {
    int *data;
    size_t count;
    size_t capacity;
};

/*
 * Creates a new, empty stack.
 * First allocates memory for the stack structure.
 * Then allocates memory for an array of ISTACK_START integers.
 * Initializes count to 0 because the stack starts empty.
 * Returns a pointer to the new stack on success.
 * Returns NULL if memory allocation fails.
 */
istack_t *istack_create(void)
{
    /* Allocate enough memory for the stack structure. */
    istack_t *s = malloc(sizeof(*s));

    /* Check whether the structure allocation failed. */
    if (s == NULL) {
        return NULL;
    }

    /* Allocate space for the initial 8 integers. */
    s->data = malloc(ISTACK_START * sizeof(*s->data));

    /* If allocation fails, free the structure to avoid a leak. */
    if (s->data == NULL) {
        free(s);
        return NULL;
    }

    /* The new stack contains no values yet. */
    s->count = 0;

    /* The initial capacity is 8 integers. */
    s->capacity = ISTACK_START;

    /* Return the newly created stack. */
    return s;
}

/*
 * Destroys a stack and releases its allocated memory.
 * The integer array and the stack structure are both
 * dynamically allocated, so both must be freed.
 * Passing NULL is allowed and does nothing.
 */
void istack_destroy(istack_t *s)
{
    /* Do nothing if the stack pointer is NULL. */
    if (s == NULL) {
        return;
    }

    /* Free the dynamically allocated integer array. */
    free(s->data);

    /* Free the memory allocated for the stack structure. */
    free(s);
}

/*
 * Pushes an integer onto the top of the stack.
 * If the stack has available space, the value is added.
 * If the stack is full, its capacity is doubled using realloc.
 * A temporary pointer is used with realloc so that
 * an allocation failure does not lose the original array.
 * Returns 0 on success.
 * Returns -1 if the stack is NULL or memory allocation fails.
 */
int istack_push(istack_t *s, int v)
{
    /* Cannot push a value onto a nonexistent stack. */
    if (s == NULL) {
        return -1;
    }

    /* Check whether all available array positions are occupied. */
    if (s->count == s->capacity) {

        /* Prevent overflow when doubling the capacity. */
        if (s->capacity > SIZE_MAX / 2) {
            return -1;
        }

        /* Calculate the new capacity by doubling the old one. */
        size_t new_capacity = s->capacity * 2;

        /* Prevent overflow when calculating allocation size. */
        if (new_capacity > SIZE_MAX / sizeof(*s->data)) {
            return -1;
        }

        /*
         * Request a larger memory block.
         * realloc preserves the existing values if successful.
         * new_data temporarily holds the returned pointer.
         */
        int *new_data = realloc(
            s->data,
            new_capacity * sizeof(*s->data)
        );

        /*
         * If realloc fails, the original array still exists.
         * Return -1 without modifying the stack.
         */
        if (new_data == NULL) {
            return -1;
        }

        /* Update the array pointer to the resized allocation. */
        s->data = new_data;

        /* Record the new capacity of the stack. */
        s->capacity = new_capacity;
    }

    /* Store the new integer in the next available array position. */
    s->data[s->count] = v;

    /* Increase the number of integers currently stored. */
    s->count++;

    /* Indicate that the push operation succeeded. */
    return 0;
}

/*
 * Removes the integer currently at the top of the stack.
 * The top integer is the most recently pushed value.
 * If v is not NULL, the removed value is copied into *v.
 * Returns 0 on success.
 * Returns -1 if the stack is NULL or empty.
 * On failure, the value pointed to by v is not modified.
 */
int istack_pop(istack_t *s, int *v)
{
    /* Reject NULL stacks and stacks containing no integers. */
    if (s == NULL || s->count == 0) {
        return -1;
    }

    /* Copy the top value into *v if the caller requested it. */
    if (v != NULL) {
        *v = s->data[s->count - 1];
    }

    /*
     * Decrease the count by one to remove the top value.
     * The array's allocated capacity remains unchanged.
     */
    s->count--;

    /* Indicate that the pop operation succeeded. */
    return 0;
}

/*
 * Reads the integer at the top of the stack.
 * Unlike pop, peek does not remove the value.
 * If v is not NULL, the top integer is copied into *v.
 * Returns 0 on success.
 * Returns -1 if the stack is NULL or empty.
 */
int istack_peek(const istack_t *s, int *v)
{
    /* Cannot peek at a NULL or empty stack. */
    if (s == NULL || s->count == 0) {
        return -1;
    }

    /* Copy the top integer without removing it. */
    if (v != NULL) {
        *v = s->data[s->count - 1];
    }

    /* Count and capacity remain unchanged. */
    return 0;
}

/*
 * Returns the number of integers currently in the stack.
 * The count represents how many array positions
 * contain active stack values.
 * Returns 0 if the stack pointer is NULL.
 */
size_t istack_count(const istack_t *s)
{
    /* A NULL stack is treated as containing zero values. */
    if (s == NULL) {
        return 0;
    }

    /* Return the number of values currently stored. */
    return s->count;
}

/*
 * Checks whether the stack is empty.
 * An empty stack has count equal to zero.
 * Returns true if the stack is empty or NULL.
 * Returns false if the stack contains any integers.
 */
bool istack_is_empty(const istack_t *s)
{
    /* Treat a NULL stack as empty. */
    if (s == NULL) {
        return true;
    }

    /* This comparison is true when count equals zero. */
    return s->count == 0;
}

/*
 * Returns the current capacity of the stack.
 * Capacity represents how many integers could fit
 * in the allocated array without requiring realloc.
 * Returns 0 if the stack pointer is NULL.
 */
size_t istack_capacity(const istack_t *s)
{
    /* A NULL stack has no allocated capacity. */
    if (s == NULL) {
        return 0;
    }

    /* Return the current number of available array slots. */
    return s->capacity;
}

/*
 * Creates a separate array containing all stack values.
 * Values are copied from bottom to top, preserving
 * the order in which they were originally pushed.
 * The number of copied integers is written into *n.
 * Returns a pointer to the new array on success.
 * Returns NULL for a NULL or empty stack, allocation
 * failure, or invalid n pointer.
 * The caller is responsible for freeing the new array.
 */
int *istack_to_array(const istack_t *s, size_t *n)
{
    /* A valid n pointer is needed to return the array length. */
    if (n == NULL) {
        return NULL;
    }

    /* Initialize the output length to zero. */
    *n = 0;

    /* An empty or NULL stack has no values to copy. */
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    /* Prevent overflow when calculating the allocation size. */
    if (s->count > SIZE_MAX / sizeof(*s->data)) {
        return NULL;
    }

    /* Allocate enough memory to hold all active stack values. */
    int *array = malloc(s->count * sizeof(*array));

    /* Return NULL if memory allocation was unsuccessful. */
    if (array == NULL) {
        return NULL;
    }

    /*
     * Copy each integer into the newly allocated array.
     * Index 0 contains the bottom value.
     * Index count - 1 contains the top value.
     */
    for (size_t i = 0; i < s->count; i++) {
        array[i] = s->data[i];
    }

    /* Tell the caller how many integers were copied. */
    *n = s->count;

    /* Return the independent copy of the stack's values. */
    return array;
}
