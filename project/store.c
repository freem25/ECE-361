
#include "store.h"
#include <stddef.h>

/* Temporary capacity until the tree is implemented. */
#define STORE_CAPACITY 1024

/* Private storage structure. */
struct store {
    record_t records[STORE_CAPACITY];
    int count;
    int in_use;
};

/* One static store, no dynamic allocation needed yet. */
static struct store storage;

/* Initialize the storage module. */
void store_init(void)
{
    storage.count = 0;
    storage.in_use = 0;
}

/* Convert humidity to a whole-percent key. */
static int humidity_key(float humidity)
{
    return (int)humidity;
}

/* Compare records by quantized humidity. */
int record_compare(const record_t *a,
                   const record_t *b)
{
    int key_a = humidity_key(a->reading.humidity);
    int key_b = humidity_key(b->reading.humidity);

    return (key_a > key_b) - (key_a < key_b);
}

/* Create an empty store. */
store_t *store_create(void)
{
    if (storage.in_use) {
        return NULL;
    }

    storage.count = 0;
    storage.in_use = 1;

    return &storage;
}

/* Insert a record in sorted order. */
int store_insert(store_t *store,
                 const record_t *record)
{
    if (store == NULL || record == NULL ||
        !store->in_use) {
        return -1;
    }

    if (store->count >= STORE_CAPACITY) {
        return -1;
    }

    int pos = 0;

    /* Find the insertion position. */
    while (pos < store->count &&
           record_compare(&store->records[pos],
                          record) <= 0) {
        pos++;
    }

    /* Shift later records one position right. */
    for (int i = store->count; i > pos; i--) {
        store->records[i] = store->records[i - 1];
    }

    /* Insert the complete record. */
    store->records[pos] = *record;
    store->count++;

    return 0;
}

/* Return the number of stored records. */
int store_count(const store_t *store)
{
    if (store == NULL || !store->in_use) {
        return 0;
    }

    return store->count;
}

/* Release the store so it can be created again. */
void store_destroy(store_t *store)
{
    if (store == NULL) {
        return;
    }

    store->count = 0;
    store->in_use = 0;
}
