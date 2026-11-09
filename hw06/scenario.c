#include "scenario.h"

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Find the scenario values active at a particular tick. */
const scenario_line_t *scenario_at_tick(
    const scenario_t *s, uint32_t tick)
{
    /* Check that the scenario pointer is valid. */
    if (s == NULL || s->count == 0) {
        return NULL;
    }

    /* No matching line has been found yet. */
    const scenario_line_t *active = NULL;

    /* Examine scenario lines in increasing tick order. */
    for (size_t i = 0; i < s->count; i++) {

        /* Stop when we reach a future scenario line. */
        if (s->lines[i].tick > tick) {
            break;
        }

        /* Remember the most recent applicable line. */
        active = &s->lines[i];
    }

    /* Return the values currently in force. */
    return active;
}


/* Load a scenario file into the supplied structure. */
int scenario_load(const char *filename, scenario_t *s)
{
    /* Reject invalid arguments. */
    if (filename == NULL || s == NULL) {
        return -1;
    }

    /* Open the scenario file for reading. */
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        return -1;
    }

    char line[512];
    int line_number = 0;
    int have_start = 0;
    int bad_line = 0;

    /* Begin with an empty scenario. */
    s->count = 0;
    s->start = (time_t)0;

    /* Read one line at a time. */
    while (fgets(line, sizeof(line), fp) != NULL) {

        line_number++;

        /* Reject lines too long for the buffer. */
        size_t len = strlen(line);
        if (len == sizeof(line) - 1 &&
            line[len - 1] != '\n') {
            bad_line = line_number;
            break;
        }

        /* Remove comments beginning with #. */
        char *comment = strchr(line, '#');
        if (comment != NULL) {
            *comment = '\0';
        }

        /* Skip leading whitespace. */
        char *p = line;
        while (isspace((unsigned char)*p)) {
            p++;
        }

        /* Ignore blank lines and comment-only lines. */
        if (*p == '\0') {
            continue;
        }

        /* First meaningful line must contain start time. */
        if (!have_start) {

            long long timestamp;
            char extra;

            if (strncmp(p, "start", 5) != 0 ||
                !isspace((unsigned char)p[5]) ||
                sscanf(p + 5, "%lld %c",
                       &timestamp, &extra) != 1) {
                bad_line = line_number;
                break;
            }

            /* Reject timestamps that do not fit time_t. */
            s->start = (time_t)timestamp;

            if ((long long)s->start != timestamp) {
                bad_line = line_number;
                break;
            }

            have_start = 1;
            continue;
        }

        /* Parse one scenario data line. */
        unsigned long long tick;
        unsigned long long switches;
        double temp;
        double rh;
        char extra;

        int fields = sscanf(p, "%llu %lf %lf %llx %c",
                            &tick, &temp, &rh,
                            &switches, &extra);

        /* Require exactly four valid fields. */
        if (fields != 4 ||
            tick > UINT32_MAX ||
            switches > UINT32_MAX ||
            !isfinite(temp) ||
            !isfinite(rh) ||
            temp > FLT_MAX || temp < -FLT_MAX ||
            rh > FLT_MAX || rh < -FLT_MAX) {
            bad_line = line_number;
            break;
        }

        /* Reject scenarios with more than 1000 entries. */
        if (s->count >= SCENARIO_MAX_LINES) {
            bad_line = line_number;
            break;
        }

        /* Require strictly increasing tick numbers. */
        if (s->count > 0 &&
            tick <= s->lines[s->count - 1].tick) {
            bad_line = line_number;
            break;
        }

        /* Store this scenario entry. */
        scenario_line_t *entry = &s->lines[s->count];

        entry->tick = (uint32_t)tick;
        entry->temp_c = (float)temp;
        entry->rh_pct = (float)rh;
        entry->switches = (uint32_t)switches;

        s->count++;
    }

    /* Detect a file-reading error. */
    int read_error = ferror(fp);

    /* Close the file when finished. */
    fclose(fp);

    if (read_error) {
        return -1;
    }

    /* Return the first invalid physical line number. */
    if (bad_line != 0) {
        return bad_line;
    }

    /* A valid scenario needs a start and data lines. */
    if (!have_start || s->count == 0) {
        return line_number + 1;
    }

    return 0;
}
