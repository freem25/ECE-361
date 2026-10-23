
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include <time.h>

/* One sensor reading with its timestamp,
   temperature in Celsius, and relative humidity. */
typedef struct {
    time_t timestamp;
    float temperature;
    float humidity;
} reading_t;

/* Initialize the sensor module. */
void sensor_init(void);

/* Read temperature and humidity from the sensor.
   Returns status through the int return value. */
int sensor_read(uint32_t *base,
                float *temp_c,
                float *rh_pct);

#endif
#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

/* Initialize the sensor module. */
void sensor_init(void);

int sensor_read(uint32_t *base,
		float *temp_c,
		float *rh_pct);

#endif
