
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "sensor.h"
#include "panel.h"
#include "scenario.h"
#include "iom361_r4.h"

int main(int argc, char *argv[])
{
    int rc;

    /* Require exactly one scenario filename. */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s scenario.txt\n", argv[0]);
        return 1;
    }

    /* Load the scenario into memory. */
    scenario_t scenario;

    rc = scenario_load(argv[1], &scenario);

    if (rc != 0) {
        fprintf(stderr, "Scenario load failed: %d\n", rc);
        return 1;
    }

    /* Initialize four switches and four LEDs. */
    uint32_t *base = iom361_initialize(4, 4, &rc);

    if (base == NULL || rc != 0) {
        fprintf(stderr, "iom361 initialization failed\n");
        return 1;
    }

    /* Disable automatic LED display output. */
    iom361_set_display(false);

    /* Track the previous switch positions. */
    uint32_t before = 0;

    /* Find the final tick in the scenario. */
    uint32_t last_tick =
        scenario.lines[scenario.count - 1].tick;

    /* Process every tick from zero through the last tick. */
    for (uint32_t tick = 0; ; tick++) {

        /* Find the scenario values active at this tick. */
        const scenario_line_t *line =
            scenario_at_tick(&scenario, tick);

        if (line == NULL) {
            fprintf(stderr, "No scenario values at tick %u\n",
                    (unsigned)tick);
            return 1;
        }

        /* Set the simulated world's sensor and switches. */
        _iom361_set_sensor1(line->temp_c, line->rh_pct);
        _iom361_set_switches(line->switches);

        /* Calculate the current Unix timestamp. */
        time_t now = scenario.start + (time_t)tick;

        /* Read the temperature and humidity registers. */
        reading_t reading;

        rc = sensor_read(base, now, &reading);

        if (rc != 0) {
            fprintf(stderr, "Sensor read failed: %d\n", rc);
            return 1;
        }

        /* Read the current switch register. */
        uint32_t switches =
            iom361_readReg(base, SWITCHES_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "Switch read failed: %d\n", rc);
            return 1;
        }

        /* Detect switches newly pressed this tick. */
        uint32_t pressed = panel_pressed(switches, before);

        /* Toggle the LED for each newly pressed switch. */
        for (int k = 0; k < 4; k++) {
            if (pressed & (1u << k)) {
                rc = panel_toggle(base, k);

                if (rc != 0) {
                    fprintf(stderr, "LED toggle failed: %d\n", rc);
                    return 1;
                }
            }
        }

        /* Read the resulting LED states. */
        uint32_t leds =
            iom361_readReg(base, LEDS_REG, &rc);

        if (rc != 0) {
            fprintf(stderr, "LED read failed: %d\n", rc);
            return 1;
        }

        /* Convert the timestamp to a UTC date and time. */
        struct tm *utc = gmtime(&reading.t);
        char time_string[32];

        if (utc == NULL ||
            strftime(time_string, sizeof(time_string),
                     "%Y-%m-%d %H:%M:%S", utc) == 0) {
            fprintf(stderr, "Time formatting failed\n");
            return 1;
        }

        /* Print one trace line for this tick. */
        printf("%s temp=%.1f rh=%.1f switches=0x%X "
               "pressed=0x%X leds=0x%X\n",
               time_string,
               reading.temp_c,
               reading.rh_pct,
               (unsigned)switches,
               (unsigned)pressed,
               (unsigned)leds);

        /* Save switch positions for the next tick. */
        before = switches;

        /* Stop after processing the final tick. */
        if (tick == last_tick) {
            break;
        }
    }

    return 0;
}
