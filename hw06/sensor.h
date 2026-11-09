
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/* Stores one sensor reading and its timestamp. */
typedef struct {
    time_t t;       /* Timestamp */
    float temp_c;   /* Temperature in Celsius */
    float rh_pct;   /* Relative humidity percentage */
} reading_t;

/* Reads temperature and humidity from iom361.
   Returns 0 on success or an error code on failure.
   On failure, the reading remains unchanged. */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
