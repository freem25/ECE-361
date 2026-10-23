
#include "tally.h"
#include <string.h>

/*
 * Each entry stores a name and its count.
 * We use TALLY_LEN + 1 to leave room for '\0',
 * the null terminator at the end of a C string.
 */
struct entry {
    char name[TALLY_LEN + 1];
    int count;
};

/*
 * This is the actual definition of the opaque tally.
 * It is only visible inside tally.c.
 *
 * entries: array containing the names and counts.
 * size: number of different names currently stored.
 * in_use: 1 if the tally is occupied, 0 if available.
 */
struct tally {
    struct entry entries[TALLY_NAMES];
    int size;
    int in_use;
};

/*
 * Create a fixed pool of four tallies.
 * Static storage means we do not need malloc().
 */
static struct tally tallies[TALLY_MAX];

/*
 * Creates a new empty tally by finding an unused slot.
 */
tally_t *tally_create(void)
{
    /* Check each slot in the static tally array. */
    for (int i = 0; i < TALLY_MAX; i++) {

        /* Find a slot that is not being used. */
        if (!tallies[i].in_use) {

            /* Mark the slot as occupied. */
            tallies[i].in_use = 1;

            /* Start the new tally with zero names. */
            tallies[i].size = 0;

            /* Return the address of this tally. */
            return &tallies[i];
        }
    }

    /* All four tally slots are occupied. */
    return NULL;
}

/*
 * Releases a tally so its slot can be used again.
 */
void tally_destroy(tally_t *t)
{
    /* A NULL pointer has nothing to destroy. */
    if (t == NULL) {
        return;
    }

    /* Reset the number of stored names. */
    t->size = 0;

    /* Mark this tally as available again. */
    t->in_use = 0;
}

/*
 * Adds one occurrence of a name.
 * Existing names have their counts increased.
 * New names are inserted in alphabetical order.
 */
int tally_add(tally_t *t, const char *name)
{
    /* Reject NULL pointers before using them. */
    if (t == NULL || name == NULL) {
        return -1;
    }

    /* Find how many characters are in the name. */
    size_t len = strlen(name);

    /* Reject empty names or names longer than 15. */
    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    /* Start searching at the beginning of the array. */
    int pos = 0;

    /*
     * Search for the existing name or the correct
     * alphabetical position for a new name.
     */
    while (pos < t->size) {

        /*
         * strcmp compares two strings.
         * Negative: stored name comes first.
         * Zero: names are the same.
         * Positive: new name comes first.
         */
        int cmp = strcmp(t->entries[pos].name, name);

        /* If the name exists, increase its count. */
        if (cmp == 0) {
            t->entries[pos].count++;

            /* Return the updated count. */
            return t->entries[pos].count;
        }

        /*
         * If cmp is positive, the new name belongs
         * before the name at this position.
         */
        if (cmp > 0) {
            break;
        }

        /* Otherwise, keep searching the next entry. */
        pos++;
    }

    /*
     * If we reach here, the name is new.
     * Reject it if all 16 name slots are occupied.
     */
    if (t->size >= TALLY_NAMES) {
        return -1;
    }

    /*
     * Move existing entries one position to the right.
     * We start at the end to avoid overwriting data.
     *
     * Example:
     * Before: apple, door, zebra
     * Insert: motion
     * After:  apple, door, motion, zebra
     */
    for (int i = t->size; i > pos; i--) {
        t->entries[i] = t->entries[i - 1];
    }

    /*
     * Copy the new name into its correct position.
     * len + 1 includes the terminating '\0'.
     * We already checked that the name fits.
     */
    memcpy(t->entries[pos].name, name, len + 1);

    /* A newly added name starts with a count of one. */
    t->entries[pos].count = 1;

    /* Increase the number of different stored names. */
    t->size++;

    /* Return the new name's count. */
    return 1;
}

/*
 * Searches for a name and returns its current count.
 */
int tally_count(const tally_t *t, const char *name)
{
    /* NULL inputs cannot be searched. */
    if (t == NULL || name == NULL) {
        return 0;
    }

    /* Check each stored name. */
    for (int i = 0; i < t->size; i++) {

        /* strcmp returns zero for matching strings. */
        if (strcmp(t->entries[i].name, name) == 0) {
            return t->entries[i].count;
        }
    }

    /* Return zero if the name was not found. */
    return 0;
}

/*
 * Returns the number of different names in a tally.
 */
int tally_size(const tally_t *t)
{
    /* A NULL tally contains no names. */
    if (t == NULL) {
        return 0;
    }

    /* Return the number of distinct stored names. */
    return t->size;
}

/*
 * Prints every stored name and its count.
 * Names are already sorted by tally_add().
 */
void tally_print(const tally_t *t, FILE *out)
{
    /* Avoid using NULL pointers. */
    if (t == NULL || out == NULL) {
        return;
    }

    /* Print each name and its corresponding count. */
    for (int i = 0; i < t->size; i++) {

        /*
         * %s prints the string.
         * %d prints its integer count.
         * \n moves to a new line.
         */
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}
