
#include <stdio.h>
#include <string.h>
#include "../tally.h"

/*
 * Keep track of how many tests were executed
 * and how many of those tests failed.
 *
 * static limits these variables to this source file.
 */
static int tests_run = 0;
static int tests_failed = 0;

/*
 * Check whether a test condition is true.
 *
 * Takes:
 *   condition - nonzero means the test passed.
 *   name - description of the test being checked.
 *
 * Prints PASS or FAIL and updates the counters.
 */
static void check(int condition, const char *name)
{
    /* Every call represents one completed test. */
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

/*
 * Main test program.
 * Tests each required operation of the tally ADT,
 * including normal behavior and boundary cases.
 */
int main(void)
{
    /* Create the tally used for the first tests. */
    tally_t *t = tally_create();

    /* Stop if the initial tally cannot be created. */
    if (t == NULL) {
        printf("FAIL: Initial tally creation\n");
        return 1;
    }

    /*
     * TEST 1: Counting up.
     * Adding the same name repeatedly should
     * increase its count from 1 to 2 to 3.
     */
    check(tally_add(t, "door") == 1 &&
          tally_add(t, "door") == 2 &&
          tally_add(t, "door") == 3 &&
          tally_count(t, "door") == 3,
          "Counting up");

    /*
     * TEST 2: Unknown name.
     * A name that was never added should return 0.
     */
    check(tally_count(t, "unknown") == 0,
          "Unknown name returns zero");

    /*
     * TEST 3: Empty string.
     * The assignment does not allow empty names.
     */
    check(tally_add(t, "") == -1,
          "Empty name rejected");

    /*
     * TEST 4: Maximum valid name length.
     * A 15-character name should be accepted.
     */
    check(tally_add(t, "123456789012345") == 1,
          "15-character name accepted");

    /*
     * TEST 5: Name longer than the limit.
     * A 16-character name should be rejected.
     */
    check(tally_add(t, "1234567890123456") == -1,
          "16-character name rejected");

    /*
     * TEST 6: NULL arguments.
     * Each function should handle the NULL
     * cases specified by the assignment.
     */
    check(tally_add(NULL, "door") == -1 &&
          tally_add(t, NULL) == -1 &&
          tally_count(NULL, "door") == 0 &&
          tally_count(t, NULL) == 0 &&
          tally_size(NULL) == 0,
          "NULL arguments handled");

    /* Release the first tally before capacity tests. */
    tally_destroy(t);

    /* Create an empty tally for the next tests. */
    t = tally_create();

    int inserted = (t != NULL);

    if (t == NULL) {
        printf("FAIL: Cannot recreate tally\n");
        return 1;
    }

    /*
     * Fill the tally with 16 different names.
     * snprintf generates name00 through name15.
     */
    for (int i = 0; i < TALLY_NAMES; i++) {
        char name[TALLY_LEN + 1];

        snprintf(name, sizeof(name), "name%02d", i);

        if (tally_add(t, name) != 1) {
            inserted = 0;
        }
    }

    /*
     * TEST 7: Maximum number of different names.
     * One tally should hold exactly 16 names.
     */
    check(inserted &&
          tally_size(t) == TALLY_NAMES,
          "Tally holds 16 names");

    /*
     * TEST 8: Full tally rejects new names.
     * A seventeenth different name cannot fit.
     */
    check(tally_add(t, "extra") == -1,
          "Full tally rejects new name");

    /*
     * TEST 9: Existing names still count when full.
     * Adding an existing name does not need
     * another slot in the entries array.
     */
    check(tally_add(t, "name00") == 2 &&
          tally_count(t, "name00") == 2,
          "Full tally counts existing name");

    /* Release the tally before testing the pool. */
    tally_destroy(t);

    /*
     * Create five tallies.
     * Only four should be available simultaneously.
     */
    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_t *c = tally_create();
    tally_t *d = tally_create();
    tally_t *e = tally_create();

    /*
     * TEST 10: Maximum simultaneous tallies.
     * The first four should succeed.
     * The fifth should return NULL.
     */
    check(a != NULL && b != NULL &&
          c != NULL && d != NULL &&
          e == NULL,
          "Maximum four tallies");

    /*
     * Release tally b and create another tally.
     * This checks whether a destroyed slot can
     * be reused by tally_create.
     */
    tally_destroy(b);
    b = tally_create();

    /*
     * TEST 11: Create after destroy.
     * The newly available slot should work.
     */
    check(b != NULL,
          "Create after destroy");

    /*
     * TEST 12: Independent tallies.
     * Adding the same name to two tallies should
     * produce separate counts in each one.
     */
    if (a != NULL && b != NULL) {
        tally_add(a, "sensor");
        tally_add(b, "sensor");
        tally_add(b, "sensor");

        check(tally_count(a, "sensor") == 1 &&
              tally_count(b, "sensor") == 2,
              "Independent tallies");
    } else {
        check(0, "Independent tallies");
    }

    /* Release all tally slots. */
    tally_destroy(a);
    tally_destroy(b);
    tally_destroy(c);
    tally_destroy(d);
    tally_destroy(e);

    /* Check that destroying NULL does nothing. */
    tally_destroy(NULL);

    /*
     * TEST 13: Alphabetical output.
     * Names are deliberately added out of order.
     * tally_print should print them in sorted order.
     */
    t = tally_create();

    if (t == NULL) {
        printf("FAIL: Cannot create print test tally\n");
        return 1;
    }

    tally_add(t, "zebra");
    tally_add(t, "apple");
    tally_add(t, "middle");

    /*
     * tmpfile creates a temporary file.
     * This lets us capture the output of tally_print
     * and compare it with the expected text.
     */
    FILE *out = tmpfile();

    if (out == NULL) {
        check(0, "Print order (temporary file)");
    } else {
        char output[256] = {0};

        /* Write the tally output to the file. */
        tally_print(t, out);

        /* Move back to the beginning of the file. */
        rewind(out);

        /*
         * Read the printed text into output.
         * Reserve the last byte for '\0'.
         */
        size_t n = fread(output, 1,
                         sizeof(output) - 1, out);
        output[n] = '\0';

        /*
         * strcmp returns zero when strings match.
         * Check that the exact printed output
         * matches the expected alphabetical order.
         */
        check(strcmp(output,
              "apple 1\nmiddle 1\nzebra 1\n") == 0,
              "Alphabetical print order");

        /* Close the temporary file. */
        fclose(out);
    }

    /* Release the final tally. */
    tally_destroy(t);

    /*
     * Print the results from all 13 test checks.
     */
    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n",
           tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    /*
     * Return zero if all tests passed.
     * Return one if at least one test failed,
     * allowing Make to recognize a test failure.
     */
    return tests_failed == 0 ? 0 : 1;
}
