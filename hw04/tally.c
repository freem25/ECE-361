
#include "tally.h"
#include <string.h>

/* One entry stores a name and its count. */
struct entry {
    char name[TALLY_LEN + 1];
    int count;
};

/* The complete tally structure is private. */
struct tally {
    struct entry entries[TALLY_NAMES];
    int size;
    int in_use;
};

/* Static storage for at most TALLY_MAX tallies. */
static struct tally tallies[TALLY_MAX];

/* Create an empty tally. */
tally_t *tally_create(void)
{
    for (int i = 0; i < TALLY_MAX; i++) {
        if (!tallies[i].in_use) {
            tallies[i].in_use = 1;
            tallies[i].size = 0;
            return &tallies[i];
        }
    }

    return NULL;
}

/* Release a tally for reuse. */
void tally_destroy(tally_t *t)
{
    if (t == NULL) {
        return;
    }

    t->size = 0;
    t->in_use = 0;
}

/* Add one occurrence of a name. */
int tally_add(tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return -1;
    }

    size_t len = strlen(name);

    if (len == 0 || len > TALLY_LEN) {
        return -1;
    }

    int pos = 0;

    /* Find the name or its insertion position. */
    while (pos < t->size) {
        int cmp = strcmp(t->entries[pos].name, name);

        if (cmp == 0) {
            t->entries[pos].count++;
            return t->entries[pos].count;
        }

        if (cmp > 0) {
            break;
        }

        pos++;
    }

    /* A new name needs an available slot. */
    if (t->size >= TALLY_NAMES) {
        return -1;
    }

    /* Move later entries right to make space. */
    for (int i = t->size; i > pos; i--) {
        t->entries[i] = t->entries[i - 1];
    }

    /* Copy the new name, including its '\0'. */
    memcpy(t->entries[pos].name, name, len + 1);

    t->entries[pos].count = 1;
    t->size++;

    return 1;
}

/* Count occurrences of an existing name. */
int tally_count(const tally_t *t, const char *name)
{
    if (t == NULL || name == NULL) {
        return 0;
    }

    for (int i = 0; i < t->size; i++) {
        if (strcmp(t->entries[i].name, name) == 0) {
            return t->entries[i].count;
        }
    }

    return 0;
}

/* Return the number of different names. */
int tally_size(const tally_t *t)
{
    if (t == NULL) {
        return 0;
    }

    return t->size;
}

/* Print names and counts in alphabetical order. */
void tally_print(const tally_t *t, FILE *out)
{
    if (t == NULL || out == NULL) {
        return;
    }

    for (int i = 0; i < t->size; i++) {
        fprintf(out, "%s %d\n",
                t->entries[i].name,
                t->entries[i].count);
    }
}
