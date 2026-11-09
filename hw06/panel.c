
#include "panel.h"
#include "iom361_r4.h"

/* Detect switches that changed from OFF (0) to ON (1). */
uint32_t panel_pressed(uint32_t now, uint32_t before)
{
    /* A pressed switch is ON now but was OFF before. */
    return now & ~before;
}

/* Detect switches that changed from ON (1) to OFF (0). */
uint32_t panel_released(uint32_t now, uint32_t before)
{
    /* A released switch was ON before but is OFF now. */
    return before & ~now;
}

/* Turn LED n on or off without changing the other LEDs. */
int panel_led(uint32_t *base, int n, int on)
{
    int rc;

    /* Check that LED number is between 0 and 31. */
    if (n < 0 || n > 31) {
        return -1;
    }

    /* Read the current LED register value. */
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    /* Stop if reading the register failed. */
    if (rc != 0) {
        return rc;
    }

    /* Create a mask for the selected LED. */
    uint32_t mask = 1u << n;

    if (on) {
        /* Turn on LED n while preserving other bits. */
        leds |= mask;
    } else {
        /* Turn off LED n while preserving other bits. */
        leds &= ~mask;
    }

    /* Write the modified value back to the LED register. */
    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}

/* Toggle LED n without changing the other LEDs. */
int panel_toggle(uint32_t *base, int n)
{
    int rc;

    /* Check that LED number is between 0 and 31. */
    if (n < 0 || n > 31) {
        return -1;
    }

    /* Read the current LED register value. */
    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    /* Stop if reading the register failed. */
    if (rc != 0) {
        return rc;
    }

    /* Create a mask for the selected LED. */
    uint32_t mask = 1u << n;

    /* Flip only the selected LED bit. */
    leds ^= mask;

    /* Write the modified value back to the register. */
    iom361_writeReg(base, LEDS_REG, leds, &rc);

    return rc;
}
