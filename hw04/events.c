
#include <stdio.h>
#include <string.h>
#include "tally.h"

/*
 * Main program:
 * Reads event names from standard input, adds them
 * to a tally, and prints the final counts.
 */
int main(void)
{
    /*
     * Create one empty tally.
     * tally_create returns NULL if no tally is available.
     */
    tally_t *t = tally_create();

    if (t == NULL) {
        fprintf(stderr, "Could not create tally\n");
        return 1;
    }

    /*
     * line: stores each line read from input.
     * skipped: counts lines that tally_add rejects.
     */
    char line[256];
    int skipped = 0;

    /*
     * Read one line at a time from standard input.
     * fgets returns NULL when there is no more input
     * or a read error occurs.
     */
    while (fgets(line, sizeof(line), stdin) != NULL) {

        /* Find the length of the text that was read. */
        size_t len = strlen(line);

        /*
         * Check whether the line ends with a newline.
         * This helps determine whether the entire line
         * was read into our buffer.
         */
        int complete = (len > 0 && line[len - 1] == '\n');

        if (complete) {
            /*
             * Remove the newline by replacing it
             * with the null terminator '\0'.
             */
            line[--len] = '\0';

        } else if (!feof(stdin)) {
            /*
             * If the line did not end with a newline,
             * read one more character to check whether
             * the input line continues.
             */
            int ch = getchar();

            if (ch != '\n' && ch != EOF) {
                /*
                 * The input line is too long for
                 * our buffer. Discard the remaining
                 * characters until newline or EOF.
                 */
                while (ch != '\n' && ch != EOF) {
                    ch = getchar();
                }

                /* Count this rejected input line. */
                skipped++;
                continue;
            }
        }

        /*
         * Ignore empty lines.
         * They should not be sent to tally_add.
         */
        if (line[0] == '\0') {
            continue;
        }

        /*
         * Ignore comment lines that begin with '#'.
         * The assignment says these lines are skipped.
         */
        if (line[0] == '#') {
            continue;
        }

        /*
         * Add the name to our tally.
         * tally_add returns -1 for invalid names or
         * when the tally cannot hold another new name.
         */
        if (tally_add(t, line) == -1) {
            skipped++;
        }
    }

    /*
     * Print each different name and its count.
     * tally_print already uses alphabetical order.
     */
    tally_print(t, stdout);

    /*
     * Print the final summary.
     * tally_size gives the number of different names.
     * skipped is the number of rejected input lines.
     */
    printf("names: %d, lines skipped: %d\n",
           tally_size(t), skipped);

    /*
     * Release the tally so its slot can be reused.
     */
    tally_destroy(t);

    /* Return zero to indicate successful execution. */
    return 0;
}
