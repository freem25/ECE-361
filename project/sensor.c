
#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>
#include <stddef.h>

/* Initialize the sensor module. */
void sensor_init(void)
{
    /* No additional initialization is required. */
}

/* Read temperature and humidity from the iom361 registers. */
int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc;

    /* Make sure the pointers are valid. */
    if (base == NULL || r == NULL) {
        return 1;
    }

    /* Read the raw temperature register. */
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    /* Read the raw humidity register. */
    uint32_t raw_hum = iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0) {
        return rc;
    }

    /* Keep only the lowest 20 bits. */
    raw_temp &= 0x000FFFFFu;
    raw_hum  &= 0x000FFFFFu;

    /* Convert raw measurements into Celsius and %RH. */
    float temp = ((float)raw_temp / 1048576.0f)
                 * 200.0f - 50.0f;

    float rh = ((float)raw_hum / 1048576.0f)
               * 100.0f;

    /* Round measurements to one decimal place. */
    temp = roundf(temp * 10.0f) / 10.0f;
    rh   = roundf(rh * 10.0f) / 10.0f;

    /* Update the reading only after both reads succeed. */
    r->timestamp = now;
    r->temperature = temp;
    r->humidity = rh;

    return 0;
}
