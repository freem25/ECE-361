
#ifndef SCENARIO_H
#define SCENARIO_H

#include <stdint.h>
#include <stddef.h>
#include <time.h>

/* Maximum number of scenario data lines. */
#define SCENARIO_MAX_LINES 1000

/* One line of scenario data. */
typedef struct {
    uint32_t tick;       /* Seconds since the start */
    float temp_c;        /* Temperature in Celsius */
    float rh_pct;        /* Relative humidity */
    uint32_t switches;   /* Switch states */
} scenario_line_t;

/* Complete scenario with its starting time. */
typedef struct {
    time_t start;        /* Unix starting timestamp */
    size_t count;        /* Number of stored data lines */
    scenario_line_t lines[SCENARIO_MAX_LINES];
} scenario_t;

/* Load and validate a scenario file.
   Returns 0 on success or the first invalid line number.
   Returns -1 if the file cannot be opened. */
int scenario_load(const char *filename, scenario_t *s);

/* Find the scenario values in force at a given tick.
   Returns NULL if no line is in force. */
const scenario_line_t *scenario_at_tick(
    const scenario_t *s, uint32_t tick);

#endif
