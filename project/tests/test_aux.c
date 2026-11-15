
#include "aux.h"

#include <stdio.h>
#include <stdlib.h>

/*
 * ECE 361 - Sauna Controller
 * S3 Circular Buffer Unit Tests
 *
 * Tests creation, insertion, retrieval,
 * capacity, overwriting, and memory cleanup.
 */

static int tests_run = 0;
static int tests_failed = 0;

/* Print PASS or FAIL for each test. */
static void check(const char *name, int passed)
{
    tests_run++;

    if (passed) {
        printf("PASS: %s\n", name);
    } else {
        printf("FAIL: %s\n", name);
        tests_failed++;
    }
}

/* Create a sample sensor reading for testing. */
static reading_t make_reading(int i)
{
    reading_t r;

    r.timestamp = (time_t)(100000 + i);
    r.temperature = (float)i;
    r.humidity = (float)(i % 100);

    return r;
}

/* Check whether a reading matches its expected values. */
static int matches(const reading_t *r, int i)
{
    return r->timestamp == (time_t)(100000 + i) &&
           r->temperature == (float)i &&
           r->humidity == (float)(i % 100);
}

/* Test creation and an initially empty buffer. */
static int test_create(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    int ok = aux_count(b) == 0 &&
             !aux_is_full(b);

    aux_destroy(b);
    return ok;
}

/* Test retrieving from an empty buffer. */
static int test_empty(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    reading_t out = make_reading(999);

    int ok = aux_get_newest(b, 0, &out) == -1 &&
             matches(&out, 999);

    aux_destroy(b);
    return ok;
}

/* Test inserting and retrieving one reading. */
static int test_one_reading(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    reading_t r = make_reading(1);
    reading_t out = {0};

    int ok = aux_add(b, &r) == 0 &&
             aux_count(b) == 1 &&
             aux_get_newest(b, 0, &out) == 0 &&
             matches(&out, 1);

    aux_destroy(b);
    return ok;
}

/* Test filling all 30 positions in the buffer. */
static int test_fill(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    int ok = 1;

    for (int i = 0; i < AUX_CAPACITY; i++) {
        reading_t r = make_reading(i);

        if (aux_add(b, &r) != 0) {
            ok = 0;
            break;
        }
    }

    ok &= aux_count(b) == AUX_CAPACITY;
    ok &= aux_is_full(b);

    aux_destroy(b);
    return ok;
}

/* Test that readings are returned newest first. */
static int test_order(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    int ok = 1;

    for (int i = 0; i < 10; i++) {
        reading_t r = make_reading(i);
        ok &= aux_add(b, &r) == 0;
    }

    for (int i = 0; i < 10; i++) {
        reading_t out = {0};

        if (aux_get_newest(b, (size_t)i, &out) != 0 ||
            !matches(&out, 9 - i)) {
            ok = 0;
        }
    }

    aux_destroy(b);
    return ok;
}

/* Test that the 31st reading overwrites the oldest. */
static int test_overwrite(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    int ok = 1;

    for (int i = 0; i <= AUX_CAPACITY; i++) {
        reading_t r = make_reading(i);
        ok &= aux_add(b, &r) == 0;
    }

    reading_t newest = {0};
    reading_t oldest = {0};

    ok &= aux_count(b) == AUX_CAPACITY;
    ok &= aux_get_newest(b, 0, &newest) == 0;
    ok &= aux_get_newest(b, AUX_CAPACITY - 1, &oldest) == 0;

    ok &= matches(&newest, 30);
    ok &= matches(&oldest, 1);

    aux_destroy(b);
    return ok;
}

/* Test repeated wraparound and retention of the latest 30. */
static int test_wraparound(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    int ok = 1;

    for (int i = 0; i < 100; i++) {
        reading_t r = make_reading(i);
        ok &= aux_add(b, &r) == 0;
    }

    ok &= aux_count(b) == 30;

    for (int i = 0; i < 30; i++) {
        reading_t out = {0};

        if (aux_get_newest(b, (size_t)i, &out) != 0 ||
            !matches(&out, 99 - i)) {
            ok = 0;
        }
    }

    aux_destroy(b);
    return ok;
}

/* Test invalid pointers and out-of-range indexes. */
static int test_invalid(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    reading_t r = make_reading(5);
    reading_t out = make_reading(999);

    int ok = 1;

    ok &= aux_add(NULL, &r) == -1;
    ok &= aux_add(b, NULL) == -1;

    ok &= aux_get_newest(NULL, 0, &out) == -1;
    ok &= aux_get_newest(b, 0, NULL) == -1;
    ok &= aux_get_newest(b, 30, &out) == -1;
    ok &= matches(&out, 999);

    ok &= aux_count(NULL) == 0;
    ok &= !aux_is_full(NULL);

    aux_destroy(NULL);
    aux_destroy(b);

    return ok;
}

/* Test that two separate buffers are independent. */
static int test_independent(void)
{
    aux_t *a = aux_create();
    aux_t *b = aux_create();

    if (a == NULL || b == NULL) {
        aux_destroy(a);
        aux_destroy(b);
        return 0;
    }

    reading_t r1 = make_reading(10);
    reading_t r2 = make_reading(20);
    reading_t out = {0};

    int ok = 1;

    ok &= aux_add(a, &r1) == 0;
    ok &= aux_add(b, &r2) == 0;

    ok &= aux_get_newest(a, 0, &out) == 0;
    ok &= matches(&out, 10);

    ok &= aux_get_newest(b, 0, &out) == 0;
    ok &= matches(&out, 20);

    aux_destroy(a);
    aux_destroy(b);

    return ok;
}

/* Test destroying a buffer that contains readings. */
static int test_destroy_nonempty(void)
{
    aux_t *b = aux_create();

    if (b == NULL) {
        return 0;
    }

    for (int i = 0; i < 50; i++) {
        reading_t r = make_reading(i);

        if (aux_add(b, &r) != 0) {
            aux_destroy(b);
            return 0;
        }
    }

    aux_destroy(b);
    return 1;
}

/* Run every test and print a summary. */
int main(void)
{
    aux_init();

    check("Create empty buffer", test_create());
    check("Read from empty buffer", test_empty());
    check("Add one reading", test_one_reading());
    check("Fill all 30 positions", test_fill());
    check("Newest-first retrieval", test_order());
    check("31st reading overwrites oldest", test_overwrite());
    check("Repeated wraparound", test_wraparound());
    check("Invalid arguments", test_invalid());
    check("Independent buffers", test_independent());
    check("Destroy nonempty buffer", test_destroy_nonempty());

    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
