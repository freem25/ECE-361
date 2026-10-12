#ifndef STATS_H
#define STATS_H

/* Returns the minimum value in an array.
   Assumes n >= 1. */
float stats_min(const float a[], int n);

/* Returns the maximum value using recursion.
   Assumes n >= 1. */
float stats_max(const float a[], int n);

/* Returns the mean of an array.
   Assumes n >= 1. */
float stats_mean(const float a[], int n);

/* Finds the longest consecutive run above threshold.
   Stores the starting index in *best_start.
   Returns the length of the longest run.
   Assumes n >= 0 and best_start is not NULL. */
int stats_longest_run(const float a[], int n,
                      float threshold, int *best_start);

#endif
