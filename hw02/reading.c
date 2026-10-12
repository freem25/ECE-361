#include "reading.h"

#include <stdio.h>

#define LINE_LEN 128

int reading_load(FILE *input, int ticks[], float temps[],
                 float hums[], int max_readings, int *skipped) {

    char line[LINE_LEN];
    int count = 0;

    *skipped = 0;

    while (fgets(line, sizeof line, input) != NULL) {
        int i = 0;

        while (line[i] == ' ' || line[i] == '\t')
            i++;

        if (line[i] == '\n' || line[i] == '\0' || line[i] == '#')
            continue;

        if (count == max_readings) {
            fprintf(stderr,
                    "warning: more than %d readings, the rest are ignored\n",
                    max_readings);
            break;
        }

        if (sscanf(line, "%d %f %f",
                   &ticks[count], &temps[count], &hums[count]) == 3)
            count++;
        else
            (*skipped)++;
    }

    return count;
}
