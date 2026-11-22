
#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include "sensor.h"

/* A record contains the sensor reading,
   sauna state, and packed status word. */
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;

/* Opaque binary search tree storage type. */
typedef struct store store_t;

/* Callback used when visiting stored records. */
typedef void (*store_visit_fn)(const record_t *record,
                               void *context);

/* Initialize the storage module.
   Kept for compatibility with main.c. */
void store_init(void);

/* Compare records by whole-percent humidity.
   Returns negative, zero, or positive. */
int record_compare(const record_t *a,
                   const record_t *b);

/* Create an empty binary search tree.
   Returns NULL if allocation fails. */
store_t *store_create(void);

/* Insert a complete record into the tree.
   Equal humidity keys are kept to the right.
   Returns 0 on success or -1 on failure. */
int store_insert(store_t *store,
                 const record_t *record);

/* Find a record matching a humidity key.
   Copies one matching record into out.
   Returns 0 if found or -1 otherwise. */
int store_find(const store_t *store,
               int humidity_key,
               record_t *out);

/* Visit every record with the matching humidity key.
   Returns the number found, or -1 on invalid arguments. */
int store_find_all(const store_t *store,
                   int humidity_key,
                   store_visit_fn visit,
                   void *context);

/* Visit all records in ascending humidity-key order. */
void store_inorder(const store_t *store,
                   store_visit_fn visit,
                   void *context);

/* Return the total number of stored records. */
int store_count(const store_t *store);

/* Return the height of the tree in nodes.
   An empty tree has height zero. */
int store_height(const store_t *store);

/* Free every node and the tree itself.
   Passing NULL does nothing. */
void store_destroy(store_t *store);

#endif
