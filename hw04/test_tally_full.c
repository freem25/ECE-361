
#include <stdio.h>
#include <string.h>
#include "tally.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Report whether a test passed or failed. */
static void check(int condition, const char *description)
{
    tests_run++;

    if (condition) {
        printf("PASS: %s\n", description);
    } else {
        printf("FAIL: %s\n", description);
        tests_failed++;
    }
}

int main(void)
{
    tally_t *t[5];

    /* TEST 1: Maximum of four tallies */
    for (int i = 0; i < 5; i++) {
        t[i] = tally_create();
    }

    check(t[0] != NULL &&
          t[1] != NULL &&
          t[2] != NULL &&
          t[3] != NULL &&
          t[4] == NULL,
          "Four tallies allowed, fifth rejected");

    /* TEST 2: Destroying frees a slot */
    tally_destroy(t[0]);

    t[0] = tally_create();

    check(t[0] != NULL,
          "Destroyed tally slot can be reused");

    /* Release all tallies before the next tests. */
    for (int i = 0; i < 5; i++) {
        tally_destroy(t[i]);
    }

    tally_t *a = tally_create();

    if (a == NULL) {
        printf("FAIL: Cannot create test tally\n");
        return 1;
    }

    /* Fill tally with 16 different names. */
    for (int i = 0; i < TALLY_NAMES; i++) {
        char name[16];
        snprintf(name, sizeof(name), "name%02d", i);
        tally_add(a, name);
    }

    check(tally_size(a) == TALLY_NAMES,
          "Tally holds 16 different names");

    /* TEST 3: Existing name increments when full */
    check(tally_add(a, "name00") == 2 &&
          tally_count(a, "name00") == 2,
          "Existing name increments when tally is full");

    /* TEST 4: Seventeenth name rejected */
    check(tally_add(a, "extra") == -1 &&
          tally_size(a) == TALLY_NAMES,
          "Seventeenth distinct name rejected");

    tally_destroy(a);
    a = tally_create();

    if (a == NULL) {
        printf("FAIL: Cannot recreate test tally\n");
        return 1;
    }

    /* TEST 5: Exactly 15 characters */
    check(tally_add(a, "123456789012345") == 1,
          "15-character name accepted");

    /* TEST 6: More than 15 characters */
    check(tally_add(a, "1234567890123456") == -1,
          "16-character name rejected");

    /* TEST 7: Invalid names */
    check(tally_add(a, NULL) == -1 &&
          tally_add(a, "") == -1 &&
          tally_add(NULL, "test") == -1,
          "NULL and empty names rejected");

    /* TEST 8: NULL tally queries */
    check(tally_count(NULL, "test") == 0 &&
          tally_count(a, NULL) == 0 &&
          tally_size(NULL) == 0,
          "NULL tally queries return zero");

    /* TEST 9: Alphabetical print order */
    tally_destroy(a);
    a = tally_create();

    if (a == NULL) {
        printf("FAIL: Cannot create print test tally\n");
        return 1;
    }

    tally_add(a, "zebra");
    tally_add(a, "apple");
    tally_add(a, "middle");

    FILE *f = tmpfile();

    if (f == NULL) {
        check(0, "Could not open temporary print file");
    } else {
        char output[256] = {0};

        tally_print(a, f);
        rewind(f);

        size_t n = fread(output, 1, sizeof(output) - 1, f);
        output[n] = '\0';

        check(strcmp(output,
              "apple 1\nmiddle 1\nzebra 1\n") == 0,
              "Names printed in alphabetical order");

        fclose(f);
    }

    tally_destroy(a);

    /* Final results */
    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n", tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
