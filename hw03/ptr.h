#ifndef PTR_H
#define PTR_H

/* Exchange the values pointed to by a and b. */
void swap_float(float *a, float *b);

/*
 * Stores the minimum value in *min and the maximum value in *max.
 * Returns 0 on success.
 * Returns -1 if n < 1 or any pointer is NULL.
 */
int minmax(const float *a, int n, float *min, float *max);

/*
 * Returns a pointer to the first element greater than threshold.
 * Returns NULL if no such element exists.
 */
const float *find_above(const float *a, int n, float threshold);

/*
 * Reverses the array in place.
 * Does nothing if a is NULL or n < 2.
 */
void reverse(float *a, int n);

#endif
