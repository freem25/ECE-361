
#include "aux.h"

#include <stdlib.h>

/*
 * ECE 361 - Sauna Controller
 * Circular Buffer Implementation (S3)
 *
 * This module stores the 30 most recent sensor readings.
 *
 * When a new reading arrives:
 * - If the buffer has room, the reading is added.
 * - If the buffer is full, the oldest reading is overwritten.
 *
 * The buffer uses a fixed-size array and wraps around
 * automatically when the end of the array is reached.
 */

/*
 * Private circular buffer structure.
 *
 * readings: Array containing up to 30 sensor readings.
 *
 * head: Index where the next reading will be inserted.
 *
 * count: Number of valid readings currently stored.
 *
 * This structure is private to aux.c.
 */
struct aux {
    reading_t readings[AUX_CAPACITY];
    size_t head;
    size_t count;
};

/*
 * Initialize auxiliary functionality.
 *
 * This function is preserved for compatibility
 * with the existing main.c implementation.
 *
 * Individual buffers are initialized by aux_create().
 */
void aux_init(void)
{
    /* No global initialization is currently required. */
}

/*
 * Create a new, empty circular buffer.
 *
 * Allocates memory for the buffer structure.
 * Initializes the head and count to zero.
 *
 * Returns a pointer to the new buffer on success.
 * Returns NULL if memory allocation fails.
 */
aux_t *aux_create(void)
{
    /* Allocate memory for the entire buffer structure. */
    aux_t *buffer = malloc(sizeof(*buffer));

    /* Check whether memory allocation failed. */
    if (buffer == NULL) {
        return NULL;
    }

    /* Start inserting readings at array index 0. */
    buffer->head = 0;

    /* The new buffer contains no readings. */
    buffer->count = 0;

    /* Return the newly created circular buffer. */
    return buffer;
}

/*
 * Add a sensor reading to the circular buffer.
 *
 * Copies the reading into the next available position.
 *
 * When the buffer reaches its maximum capacity,
 * new readings automatically overwrite old readings.
 *
 * Returns 0 on success.
 * Returns -1 if either argument is NULL.
 */
int aux_add(aux_t *buffer, const reading_t *reading)
{
    /* Verify that both pointers are valid. */
    if (buffer == NULL || reading == NULL) {
        return -1;
    }

    /* Copy the sensor reading into the current head slot. */
    buffer->readings[buffer->head] = *reading;

    /*
     * Move the head to the next array position.
     *
     * The modulo operator wraps the index back to 0
     * after reaching the final array position.
     */
    buffer->head = (buffer->head + 1) % AUX_CAPACITY;

    /*
     * Increase the count until the buffer is full.
     *
     * Once 30 readings are stored, count stays at 30
     * even though older readings are overwritten.
     */
    if (buffer->count < AUX_CAPACITY) {
        buffer->count++;
    }

    /* Indicate successful insertion. */
    return 0;
}

/*
 * Retrieve the i-th newest reading.
 *
 * Index 0 means the newest reading.
 * Index 1 means the second-newest reading.
 *
 * Copies the requested reading into *out.
 *
 * Returns 0 on success.
 * Returns -1 for invalid pointers or an invalid index.
 */
int aux_get_newest(const aux_t *buffer,
                   size_t index,
                   reading_t *out)
{
    /* Check for invalid pointers. */
    if (buffer == NULL || out == NULL) {
        return -1;
    }

    /* Reject requests for readings that do not exist. */
    if (index >= buffer->count) {
        return -1;
    }

    /*
     * Calculate the array position of the requested reading.
     *
     * head points to the next insertion position,
     * so the newest reading is one position before head.
     *
     * Modulo allows the calculation to wrap around.
     */
    size_t position =
        (buffer->head + AUX_CAPACITY - 1 - index)
        % AUX_CAPACITY;

    /* Copy the requested reading to the caller. */
    *out = buffer->readings[position];

    /* Indicate success. */
    return 0;
}

/*
 * Return the number of valid readings currently stored.
 *
 * The count increases until it reaches 30.
 * It never exceeds AUX_CAPACITY.
 *
 * Returns 0 if the buffer is NULL.
 */
size_t aux_count(const aux_t *buffer)
{
    /* Treat a NULL buffer as containing zero readings. */
    if (buffer == NULL) {
        return 0;
    }

    /* Return the number of stored readings. */
    return buffer->count;
}

/*
 * Check whether the circular buffer is full.
 *
 * Returns true when 30 readings are stored.
 * Returns false otherwise.
 *
 * A NULL buffer is treated as not full.
 */
bool aux_is_full(const aux_t *buffer)
{
    /* A NULL buffer cannot be full. */
    if (buffer == NULL) {
        return false;
    }

    /* The buffer is full when count reaches capacity. */
    return buffer->count == AUX_CAPACITY;
}

/*
 * Destroy a circular buffer.
 *
 * Releases all memory allocated by aux_create().
 *
 * Since the readings array is inside the structure,
 * one free() call releases the entire allocation.
 *
 * Passing NULL is allowed and does nothing.
 */
void aux_destroy(aux_t *buffer)
{
    /* free(NULL) is safe, so no separate check is needed. */
    free(buffer);
}
