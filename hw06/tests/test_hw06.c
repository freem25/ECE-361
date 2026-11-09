
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "sensor.h"
#include "iom361_r4.h"

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
    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
