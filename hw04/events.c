
#include <stdio.h>
#include <string.h>
#include "tally.h"

int main(void)
{
    tally_t *t = tally_create();

    if (t == NULL) {
        fprintf(stderr, "Could not create tally\n");
        return 1;
    }

    char line[256];
    int skipped = 0;

    /* Read standard input one line at a time. */
    while (fgets(line, sizeof(line), stdin) != NULL) {

        /* Find the length of the line. */
        size_t len = strlen(line);

        /* Check whether the complete line was read. */
        int complete = (len > 0 && line[len - 1] == '\n');

        if (complete) {
            /* Remove the newline. */
            line[--len] = '\0';
        } else if (!feof(stdin)) {
            /* Check whether an overlong line continues. */
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                /* Consume the rest of the long line. */
                while (ch != '\n' && ch != EOF) {
                    ch = getchar();
                }

                skipped++;
                continue;
            }
        }

        /* Ignore empty lines. */
        if (line[0] == '\0') {
            continue;
        }

        /* Ignore comment lines. */
        if (line[0] == '#') {
            continue;
        }

        /* Add the name and count rejected lines. */
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    /* Print the tally in alphabetical order. */
    tally_print(t, stdout);

    /* Print the required summary. */
    printf("names: %d, lines skipped: %d\n",
           tally_size(t), skipped);

    tally_destroy(t);

    return 0;
}
