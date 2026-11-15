
#ifndef AUX_H
#define AUX_H

#include <stdbool.h>
#include <stddef.h>
#include "sensor.h"

/*
 * ECE 361 - Sauna Controller
 * Auxiliary Circular Buffer (S3)
 *
 * Stores the 30 most recent sensor readings.
 * When full, adding a new reading overwrites the oldest.
 */

/* Maximum number of readings the buffer can hold. */
#define AUX_CAPACITY 30

/*
 * Opaque circular buffer type.
 * The actual structure is defined privately in aux.c.
 */
typedef struct aux aux_t;

/*
 * Initialize auxiliary functions.
 * Preserved for compatibility with the existing main.c.
 */
void aux_init(void);

/*
 * Create a new, empty circular buffer.
 *
 * Returns a pointer to the new buffer.
 * Returns NULL if memory allocation fails.
 */
aux_t *aux_create(void);

/*
 * Add a sensor reading to the circular buffer.
 *
 * If the buffer is full, overwrite the oldest reading.
 *
 * Returns 0 on success or -1 for invalid arguments.
 */
int aux_add(aux_t *buffer, const reading_t *reading);

/*
 * Retrieve a reading from the buffer.
 *
 * Index 0 is the newest reading.
 * Index 1 is the second-newest reading.
 *
 * Copies the requested reading into *out.
 *
 * Returns 0 on success or -1 for invalid arguments
 * or an index outside the stored readings.
 */
int aux_get_newest(const aux_t *buffer,
                   size_t index,
                   reading_t *out);

/*
 * Return the number of readings currently stored.
 *
 * Returns 0 if buffer is NULL.
 */
size_t aux_count(const aux_t *buffer);

/*
 * Check whether the circular buffer is full.
 *
 * Returns true when 30 readings are stored.
 * Returns false otherwise, including for NULL.
 */
bool aux_is_full(const aux_t *buffer);

/*
 * Destroy the circular buffer.
 *
 * Frees all dynamically allocated buffer memory.
 * Passing NULL is allowed and does nothing.
 */
void aux_destroy(aux_t *buffer);

#endif
