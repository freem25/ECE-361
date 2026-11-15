
#include "istack.h"

#include <stdio.h>
#include <stdlib.h>

/* Track the number of tests performed and failed. */
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

/* Test creating an empty stack with capacity 8. */
static int test_create(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int ok = istack_count(s) == 0 &&
             istack_capacity(s) == ISTACK_START &&
             istack_is_empty(s);

    istack_destroy(s);
    return ok;
}

/* Test that failed pop and peek leave the output unchanged. */
static int test_empty(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int value = 123;
    int ok = istack_pop(s, &value) == -1 &&
             value == 123 &&
             istack_peek(s, &value) == -1 &&
             value == 123;

    istack_destroy(s);
    return ok;
}

/* Verify last-in, first-out behavior. */
static int test_lifo(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int value = 0;
    int ok = 1;

    ok &= istack_push(s, 10) == 0;
    ok &= istack_push(s, 20) == 0;
    ok &= istack_push(s, 30) == 0;

    ok &= istack_peek(s, &value) == 0 && value == 30;
    ok &= istack_count(s) == 3;

    ok &= istack_pop(s, &value) == 0 && value == 30;
    ok &= istack_pop(s, &value) == 0 && value == 20;
    ok &= istack_pop(s, &value) == 0 && value == 10;

    ok &= istack_is_empty(s);

    istack_destroy(s);
    return ok;
}

/* Verify that the ninth push increases capacity from 8 to 16. */
static int test_growth(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int ok = 1;
    int value = 0;

    for (int i = 1; i <= 8; i++) {
        ok &= istack_push(s, i) == 0;
    }

    ok &= istack_count(s) == 8;
    ok &= istack_capacity(s) == 8;

    ok &= istack_push(s, 9) == 0;
    ok &= istack_count(s) == 9;
    ok &= istack_capacity(s) == 16;

    /* Verify all original values survived realloc. */
    for (int i = 9; i >= 1; i--) {
        ok &= istack_pop(s, &value) == 0;
        ok &= value == i;
    }

    /* Capacity must not shrink after popping. */
    ok &= istack_capacity(s) == 16;

    istack_destroy(s);
    return ok;
}

/* Verify 1000 pushes followed by 1000 pops. */
static int test_1000(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int ok = 1;
    int value = 0;

    for (int i = 0; i < 1000; i++) {
        if (istack_push(s, i) != 0) {
            ok = 0;
            break;
        }
    }

    if (ok) {
        ok &= istack_count(s) == 1000;
        ok &= istack_capacity(s) >= 1000;

        for (int i = 999; i >= 0; i--) {
            if (istack_pop(s, &value) != 0 || value != i) {
                ok = 0;
                break;
            }
        }

        ok &= istack_is_empty(s);
    }

    istack_destroy(s);
    return ok;
}

/* Verify to_array returns a bottom-first independent copy. */
static int test_to_array(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int ok = 1;

    ok &= istack_push(s, 10) == 0;
    ok &= istack_push(s, 20) == 0;
    ok &= istack_push(s, 30) == 0;

    size_t n = 0;
    int *array = istack_to_array(s, &n);

    ok &= array != NULL;
    ok &= n == 3;

    if (array != NULL && n == 3) {
        ok &= array[0] == 10;
        ok &= array[1] == 20;
        ok &= array[2] == 30;

        /* Changing the copy must not change the stack. */
        array[0] = 999;

        int value = 0;
        ok &= istack_pop(s, &value) == 0 && value == 30;
        ok &= istack_pop(s, &value) == 0 && value == 20;
        ok &= istack_pop(s, &value) == 0 && value == 10;
    }

    free(array);
    istack_destroy(s);
    return ok;
}

/* Verify NULL pointer arguments behave as specified. */
static int test_null(void)
{
    int value = 55;
    size_t n = 99;
    int ok = 1;

    ok &= istack_push(NULL, 10) == -1;
    ok &= istack_pop(NULL, &value) == -1;
    ok &= value == 55;
    ok &= istack_peek(NULL, &value) == -1;
    ok &= value == 55;

    ok &= istack_count(NULL) == 0;
    ok &= istack_is_empty(NULL);
    ok &= istack_capacity(NULL) == 0;

    ok &= istack_to_array(NULL, &n) == NULL;
    ok &= n == 0;
    ok &= istack_to_array(NULL, NULL) == NULL;

    /* Destroying NULL must be safe. */
    istack_destroy(NULL);

    return ok;
}

/* Verify optional output pointers and empty array behavior. */
static int test_optional_outputs(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    int ok = 1;
    size_t n = 99;

    ok &= istack_to_array(s, &n) == NULL;
    ok &= n == 0;

    ok &= istack_push(s, 42) == 0;
    ok &= istack_peek(s, NULL) == 0;
    ok &= istack_count(s) == 1;
    ok &= istack_to_array(s, NULL) == NULL;
    ok &= istack_pop(s, NULL) == 0;
    ok &= istack_is_empty(s);

    istack_destroy(s);
    return ok;
}

/* Verify that two stacks operate independently. */
static int test_independent(void)
{
    istack_t *a = istack_create();
    istack_t *b = istack_create();

    if (a == NULL || b == NULL) {
        istack_destroy(a);
        istack_destroy(b);
        return 0;
    }

    int ok = 1;
    int value = 0;

    ok &= istack_push(a, 10) == 0;
    ok &= istack_push(b, 20) == 0;

    ok &= istack_pop(a, &value) == 0 && value == 10;
    ok &= istack_count(a) == 0;
    ok &= istack_count(b) == 1;

    ok &= istack_pop(b, &value) == 0 && value == 20;

    istack_destroy(a);
    istack_destroy(b);
    return ok;
}

/* Verify destroying a nonempty stack is safe. */
static int test_destroy_nonempty(void)
{
    istack_t *s = istack_create();

    if (s == NULL) {
        return 0;
    }

    for (int i = 0; i < 100; i++) {
        if (istack_push(s, i) != 0) {
            istack_destroy(s);
            return 0;
        }
    }

    istack_destroy(s);
    return 1;
}

/* Run all tests and print the final summary. */
int main(void)
{
    check("Create empty stack", test_create());
    check("Empty pop and peek", test_empty());
    check("Last in, first out", test_lifo());
    check("Capacity doubles at ninth push", test_growth());
    check("1000 pushes and pops", test_1000());
    check("to_array independent copy", test_to_array());
    check("NULL arguments", test_null());
    check("Optional output pointers", test_optional_outputs());
    check("Two independent stacks", test_independent());
    check("Destroy nonempty stack", test_destroy_nonempty());

    printf("\nTests run: %d\n", tests_run);
    printf("Tests failed: %d\n", tests_failed);

    return tests_failed == 0 ? 0 : 1;
}
