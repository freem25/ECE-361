
#ifndef TALLY_H
#define TALLY_H

#include <stdio.h>

#define TALLY_MAX   4
#define TALLY_NAMES 16
#define TALLY_LEN   15

/* Opaque type: its contents are defined only in tally.c */
typedef struct tally tally_t;

/* Creates an empty tally.
   Returns a pointer to the tally, or NULL if all
   TALLY_MAX tallies are already in use. */
tally_t *tally_create(void);

/* Releases a tally so that its slot can be reused.
   Does nothing if t is NULL. */
void tally_destroy(tally_t *t);

/* Adds one occurrence of name to the tally.
   Returns the new count, or -1 for invalid input
   or if a new name cannot fit. */
int tally_add(tally_t *t, const char *name);

/* Returns the number of occurrences of name.
   Returns 0 if the name was never added or
   if t or name is NULL. */
int tally_count(const tally_t *t, const char *name);

/* Returns the number of different names stored.
   Returns 0 if t is NULL. */
int tally_size(const tally_t *t);

/* Prints each name and count to out, one per line,
   in alphabetical order using strcmp ordering. */
void tally_print(const tally_t *t, FILE *out);

#endif
