
#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>
#include <stddef.h>

/* Read temperature and humidity from the iom361 registers. */
int sensor_read(uint32_t *base, time_t now, reading_t *r)
{
    int rc;

    /* Make sure the output pointer is valid. */
    if (r == NULL) {
        return 1;
    }

    /* Read the raw temperature from the register. */
    uint32_t raw_temp = iom361_readReg(base, TEMP_REG, &rc);

    /* Stop if the temperature read failed. */
    if (rc != 0) {
        return rc;
    }

    /* Read the raw humidity from the register. */
    uint32_t raw_hum = iom361_readReg(base, HUMID_REG, &rc);

    /* Stop if the humidity read failed. */
    if (rc != 0) {
        return rc;
    }

    /* Keep only the lowest 20 bits of each reading. */
    raw_temp &= 0x000FFFFFu;
    raw_hum  &= 0x000FFFFFu;

    /* Convert raw values into Celsius and %RH. */
    float temp = ((float)raw_temp / 1048576.0f) * 200.0f - 50.0f;
    float rh   = ((float)raw_hum / 1048576.0f) * 100.0f;

    /* Round each measurement to one decimal place. */
    temp = roundf(temp * 10.0f) / 10.0f;
    rh   = roundf(rh * 10.0f) / 10.0f;

    /* Store results only after both reads succeeded. */
    r->t = now;
    r->temp_c = temp;
    r->rh_pct = rh;

    return 0;
}
