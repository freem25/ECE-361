#ifndef READING_H
#define READING_H

#include <stdio.h>

/* Reads sensor data from input.
   Stores ticks, temperatures, and humidity values
   in the provided arrays.
   Returns the number of valid readings.
   Assumes arrays have room for max_readings elements
   and skipped is not NULL. */
int reading_load(FILE *input, int ticks[], float temps[],
                 float hums[], int max_readings, int *skipped);

#endif
