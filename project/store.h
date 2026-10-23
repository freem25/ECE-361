
#ifndef STORE_H
#define STORE_H

#include <stdint.h>
#include "sensor.h"

/* A record contains a sensor reading,
   the sauna state, and a status word. */
typedef struct {
    reading_t reading;
    int state;
    uint32_t status;
} record_t;

/* Opaque storage type.
   Only store.c knows the internal structure. */
typedef struct store store_t;

/* Initialize storage for sensor readings. */
void store_init(void);

/* Compare two records by their humidity key.
   Returns negative, zero, or positive. */
int record_compare(const record_t *a,
                   const record_t *b);

/* Create a new store. */
store_t *store_create(void);

/* Insert a record into the store. */
int store_insert(store_t *store,
                 const record_t *record);

/* Return the number of stored records. */
int store_count(const store_t *store);

/* Destroy a store and release its slot. */
void store_destroy(store_t *store);

#endif

