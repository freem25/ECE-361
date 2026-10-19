#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>

/* Initialize the sensor module. */
void sensor_init(void);

int sensor_read(uint32_t *base,
		float *temp_c,
		float *rh_pct);

#endif
