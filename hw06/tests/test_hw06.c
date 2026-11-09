
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "sensor.h"
#include "iom361_r4.h"
#include "panel.h"

/* Track how many tests pass or fail. */
static int tests_run = 0;
static int tests_failed = 0;

/* Print PASS or FAIL for each test. */
static void check(int condition, const char *name)
{
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

int main(void)
{
    int rc;

    /* Initialize the simulated hardware. */
    uint32_t *base = iom361_initialize(4, 4, &rc);

    if (base == NULL || rc != 0) {
        printf("FAIL: iom361 initialization\n");
        return 1;
    }

    /* Turn off automatic LED printing. */
    iom361_set_display(false);

    /* TEST 1: Read 91.0 C and 40.0 percent humidity. */
    _iom361_set_sensor1(91.0f, 40.0f);

    reading_t r = {0};

    rc = sensor_read(base, (time_t)12345, &r);

    check(rc == 0, "Sensor read returns success");

    check(r.temp_c == 91.0f &&
          r.rh_pct == 40.0f,
          "Temperature and humidity are correct");

    check(r.t == (time_t)12345,
          "Timestamp is stored correctly");

    /* TEST 2: Failed read must not change the reading. */
    reading_t saved = {
        (time_t)100,
        25.5f,
        60.0f
    };

    /* NULL is not a valid hardware base address. */
    rc = sensor_read(NULL, (time_t)999, &saved);

    check(rc == 1, "Invalid base returns error code");

    check(saved.t == (time_t)100 &&
          saved.temp_c == 25.5f &&
          saved.rh_pct == 60.0f,
          "Failed read leaves reading unchanged");

    /* Display final test results. */

    /* ==========================================
       PART 2: SWITCH AND LED TESTS
       ========================================== */

    /* TEST 3: Detect a newly pressed switch. */
    check(panel_pressed(0x3u, 0x1u) == 0x2u,
          "Newly pressed switch detected");

    /* TEST 4: Detect a released switch. */
    check(panel_released(0x1u, 0x3u) == 0x2u,
          "Released switch detected");

    /* TEST 5: A held switch is not a new press. */
    check(panel_pressed(0x3u, 0x3u) == 0,
          "Held switch is not newly pressed");

    /* TEST 6: A held switch is not a release. */
    check(panel_released(0x3u, 0x3u) == 0,
          "Held switch is not released");

    /* TEST 7: Turn on LEDs 0 and 2. */
    int led_rc0 = panel_led(base, 0, 1);
    int led_rc2 = panel_led(base, 2, 1);

    uint32_t leds = iom361_readReg(base, LEDS_REG, &rc);

    check(led_rc0 == 0 && led_rc2 == 0 &&
          rc == 0 && leds == 0x5u,
          "Turning on two LEDs works");

    /* TEST 8: Turn on LED 1, preserving LEDs 0 and 2. */
    int panel_rc = panel_led(base, 1, 1);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check(panel_rc == 0 && rc == 0 && leds == 0x7u,
          "LED on preserves other LEDs");

    /* TEST 9: Turn off LED 1, preserving LEDs 0 and 2. */
    panel_rc = panel_led(base, 1, 0);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check(panel_rc == 0 && rc == 0 && leds == 0x5u,
          "LED off preserves other LEDs");

    /* TEST 10: Toggle LED 2 off. */
    panel_rc = panel_toggle(base, 2);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check(panel_rc == 0 && rc == 0 && leds == 0x1u,
          "Toggle turns LED off without affecting others");

    /* TEST 11: Toggle LED 2 back on. */
    panel_rc = panel_toggle(base, 2);
    leds = iom361_readReg(base, LEDS_REG, &rc);

    check(panel_rc == 0 && rc == 0 && leds == 0x5u,
          "Toggle turns LED on without affecting others");

    /* TEST 12: Reject invalid LED numbers. */
    check(panel_led(base, -1, 1) == -1 &&
          panel_toggle(base, 32) == -1,
          "Invalid LED numbers are rejected");

/* Display final results for all Part 1 and Part 2 tests. */
printf("\nTests run: %d\n", tests_run);
printf("Tests failed: %d\n", tests_failed);

return tests_failed == 0 ? 0 : 1;
}
