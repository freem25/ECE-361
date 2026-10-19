#include "sensor.h"
#include "iom361_r4.h"

#include <math.h>
#include <stdint.h>
#include <stddef.h>

void sensor_init(void)
{
    /* Nothing needed yet */
}

int sensor_read(uint32_t *base,
                float *temp_c,
                float *rh_pct)
{
    int rc;

    if (base == NULL ||
        temp_c == NULL ||
        rh_pct == NULL)
    {
        return 1;
    }

    uint32_t raw_temp =
        iom361_readReg(base, TEMP_REG, &rc);

    if (rc != 0)
    {
        return rc;
    }

    uint32_t raw_hum =
        iom361_readReg(base, HUMID_REG, &rc);

    if (rc != 0)
    {
        return rc;
    }

    float temp =
        ((float)raw_temp / 1048576.0f)
        * 200.0f - 50.0f;

    float rh =
        ((float)raw_hum / 1048576.0f)
        * 100.0f;

    temp = roundf(temp * 10.0f) / 10.0f;
    rh   = roundf(rh   * 10.0f) / 10.0f;

    *temp_c = temp;
    *rh_pct = rh;

    return 0;
}
