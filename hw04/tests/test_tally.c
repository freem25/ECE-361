
#include <stdio.h>
#include <string.h>
#include "../tally.h"

static int tests_run = 0;
static int tests_failed = 0;

/* Report the result of one test. */
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
    tally_t *t = tally_create();

    if (t == NULL) {
        printf("FAIL: Initial tally creation\n");
        return 1;
    }

    /* Counting up */
    check(tally_add(t, "door") == 1 &&
          tally_add(t, "door") == 2 &&
          tally_add(t, "door") == 3 &&
          tally_count(t, "door") == 3,
          "Counting up");

    /* Name never added */
    check(tally_count(t, "unknown") == 0,
          "Unknown name returns zero");

    /* Empty name */
    check(tally_add(t, "") == -1,
          "Empty name rejected");

    /* Exactly TALLY_LEN characters */
    check(tally_add(t, "123456789012345") == 1,
          "15-character name accepted");

    /* TALLY_LEN + 1 characters */
    check(tally_add(t, "1234567890123456") == -1,
          "16-character name rejected");

    /* NULL arguments */
    check(tally_add(NULL, "door") == -1 &&
          tally_add(t, NULL) == -1 &&
          tally_count(NULL, "door") == 0 &&
          tally_count(t, NULL) == 0 &&
          tally_size(NULL) == 0,
          "NULL arguments handled");

    tally_destroy(t);

    /* Full tally */
    t = tally_create();

    int inserted = (t != NULL);

    if (t == NULL) {
        printf("FAIL: Cannot recreate tally\n");
        return 1;
    }

    for (int i = 0; i < TALLY_NAMES; i++) {
        char name[TALLY_LEN + 1];

        snprintf(name, sizeof(name), "name%02d", i);

        if (tally_add(t, name) != 1) {
            inserted = 0;
        }
    }

    check(inserted &&
          tally_size(t) == TALLY_NAMES,
          "Tally holds 16 names");

    check(tally_add(t, "extra") == -1,
          "Full tally rejects new name");

    check(tally_add(t, "name00") == 2 &&
          tally_count(t, "name00") == 2,
          "Full tally counts existing name");

    tally_destroy(t);

    /* Maximum four simultaneous tallies */
    tally_t *a = tally_create();
    tally_t *b = tally_create();
    tally_t *c = tally_create();
    tally_t *d = tally_create();
    tally_t *e = tally_create();

    check(a != NULL && b != NULL &&
          c != NULL && d != NULL &&
          e == NULL,
          "Maximum four tallies");

    /* Destroy one and create another */
    tally_destroy(b);
    b = tally_create();

    check(b != NULL,
          "Create after destroy");

    /* Two tallies are independent */
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

    tally_destroy(a);
    tally_destroy(b);
    tally_destroy(c);
    tally_destroy(d);
    tally_destroy(e);
    tally_destroy(NULL);

    /* Print order */
    t = tally_create();

    if (t == NULL) {
        printf("FAIL: Cannot create print test tally\n");
        return 1;
    }

    tally_add(t, "zebra");
    tally_add(t, "apple");
    tally_add(t, "middle");

    FILE *out = tmpfile();

    if (out == NULL) {
        check(0, "Print order (temporary file)");
    } else {
        char output[256] = {0};

        tally_print(t, out);
        rewind(out);

        size_t n = fread(output, 1,
                         sizeof(output) - 1, out);
        output[n] = '\0';

        check(strcmp(output,
              "apple 1\nmiddle 1\nzebra 1\n") == 0,
              "Alphabetical print order");

        fclose(out);
    }

    tally_destroy(t);

    /* Summary */
    printf("\nTests run: %d\n", tests_run);
    printf("Tests passed: %d\n",
           tests_run - tests_failed);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
