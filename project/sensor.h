
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/* Stores one sensor reading and its timestamp. */
typedef struct {
    time_t timestamp;    /* Unix timestamp */
    float temperature;   /* Temperature in Celsius */
    float humidity;      /* Relative humidity percentage */
} reading_t;

/* Initialize the sensor module. */
void sensor_init(void);

/* Read temperature and humidity from iom361.
   Store the timestamp and rounded measurements in *r.
   Return 0 on success, or an error code on failure.
   On failure, the reading remains unchanged. */
int sensor_read(uint32_t *base, time_t now, reading_t *r);

#endif
