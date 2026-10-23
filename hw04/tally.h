
#ifndef TALLY_H
#define TALLY_H

/* Needed for FILE, which is used by tally_print. */
#include <stdio.h>

/* Maximum number of tallies that can exist at once. */
#define TALLY_MAX 4

/* Maximum number of different names in one tally. */
#define TALLY_NAMES 16

/* Maximum name length, not counting the null terminator. */
#define TALLY_LEN 15

/*
 * Opaque type declaration.
 * Other files can use tally_t pointers, but cannot see
 * the actual structure members defined in tally.c.
 */
typedef struct tally tally_t;

/*
 * Creates an empty tally.
 * Takes: No arguments.
 * Returns: Pointer to a new tally, or NULL if all
 *          four tally slots are already in use.
 * Assumes: Nothing.
 */
tally_t *tally_create(void);

/*
 * Destroys a tally and makes its slot available again.
 * Takes: Pointer to a tally.
 * Returns: Nothing (void).
 * Assumes: The pointer is NULL or refers to a tally
 *          previously returned by tally_create.
 *          NULL is accepted and does nothing.
 */
void tally_destroy(tally_t *t);

/*
 * Adds one occurrence of a name to the tally.
 * Takes: A tally pointer and a string containing a name.
 * Returns: The new count of that name, or -1 if invalid.
 * Assumes: Non-NULL tally pointers are valid, and
 *          non-NULL names are null-terminated strings.
 *          Names must contain 1 to 15 characters.
 */
int tally_add(tally_t *t, const char *name);

/*
 * Finds how many times a name was added.
 * Takes: A tally pointer and a name to search for.
 * Returns: The count, or 0 if not found or if an
 *          argument is NULL.
 * Assumes: Non-NULL pointers are valid.
 */
int tally_count(const tally_t *t, const char *name);

/*
 * Finds how many different names are stored.
 * Takes: A tally pointer.
 * Returns: The number of different names, or 0
 *          if the tally pointer is NULL.
 * Assumes: Non-NULL tally pointers are valid.
 */
int tally_size(const tally_t *t);

/*
 * Prints each name and its count in alphabetical order.
 * Takes: A tally pointer and an output FILE pointer.
 * Returns: Nothing (void).
 * Assumes: Non-NULL pointers are valid and the output
 *          stream is writable.
 */
void tally_print(const tally_t *t, FILE *out);

#endif

